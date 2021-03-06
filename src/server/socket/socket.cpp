//
// Created by Oliver Johnstone on 24/03/2020.
//

#include <server/socket.h>
#include <openssl/err.h>
#include <cstdint>

using namespace std;

std::string getOpenSSLError() {
    BIO *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf;
    size_t len = BIO_get_mem_data(bio, &buf);
    std::string ret(buf, len);
    BIO_free(bio);
    return ret;
}

const unsigned char serverProtocols[] = {
    2, 'h', '2',
    8, 'h', 't', 't', 'p', '/', '1', '.', '1'
};

HTTPServ::ServerSocket::ServerSocket(int port, int maxConnections) : port(port), maxConnections(maxConnections) {
}

HTTPServ::ServerSocket::ServerSocket(int port, int maxConnections, const TLS::Config* tlsConfig) : port(port), maxConnections(maxConnections), tls(tlsConfig) {
}

void HTTPServ::ServerSocket::listen() {
    if (tls) {
        initTLS();
    }

    auto opt = 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock) {
        throw SocketError("Unable to create new socket");
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        throw SocketError("Unable to set socket options");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw SocketError("Unable to bind to port");
    }

    if (::listen(sock, maxConnections) < 0) {
        throw SocketError("Unable to listen");
    }
}

HTTPServ::ServerSocket::SocketStreams HTTPServ::ServerSocket::waitForClientConnection(int socketTimeout) {
    auto addressLength = sizeof(address);
    auto client_sock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addressLength);

    if (client_sock < 0) {
        throw SocketError("Invalid client socket");
    }

    struct timeval tv{.tv_sec=socketTimeout};
    setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));

    auto listenTLS = [this, client_sock]() -> SocketStreams {
        auto ssl = SSL_new(sslCtx);
        SSL_set_fd(ssl, client_sock);

        if (SSL_accept(ssl) <= 0) {
            ::close(client_sock);
            throw SocketError("Unable to accept SSL connection: " + getOpenSSLError());
        }

        const unsigned char *alpnPtr;
        size_t alpnLen;
        ALPN alpn = ALPN::HTTP1_1;

        SSL_SESSION_get0_alpn_selected(SSL_get_session(ssl), &alpnPtr, &alpnLen);

        for (auto &[str, val] : ALPN_MAP) {
            if (strncmp(reinterpret_cast<const char *>(alpnPtr), "h2", alpnLen) == 0) {
                alpn = val;
                break;
            }
        }

        auto in = new io::stream<InSocketStream>(client_sock, ssl, alpn);
        auto out = new io::stream<OutSocketStream>(client_sock, ssl);

        return {in, out};
    };

    auto listen = [this, client_sock]() -> SocketStreams {
        auto in = new io::stream<InSocketStream>(client_sock);
        auto out = new io::stream<OutSocketStream>(client_sock);

        return {in, out};
    };

    return tls ? listenTLS() : listen();
}

void HTTPServ::ServerSocket::close() {
    ::close(sock);
}

HTTPServ::ServerSocket::~ServerSocket() {
    close();
    if (tls) {
        cleanTLS();
    }
}

void HTTPServ::ServerSocket::initTLS() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    const SSL_METHOD* method;
    method = SSLv23_server_method();

    sslCtx = SSL_CTX_new(method);
    if (!sslCtx) {
        throw SocketError("Unable to create SSL context");
    }

    SSL_CTX_set_ecdh_auto(sslCtx, 1);

    auto alpnNegFn = [](SSL *ssl, const unsigned char **out, unsigned char *outlen, const unsigned char *in, unsigned int inlen, void *protocolVector) -> int {
        auto result = SSL_select_next_proto(
            const_cast<unsigned char **>(out), outlen, serverProtocols, sizeof(serverProtocols), in, inlen
        );

        if (result == OPENSSL_NPN_NEGOTIATED) {
            return SSL_TLSEXT_ERR_OK;
        }

        return SSL_TLSEXT_ERR_NOACK;
    };

    SSL_CTX_set_alpn_select_cb(sslCtx, alpnNegFn, nullptr);

    if (SSL_CTX_use_certificate_file(sslCtx, tls->certFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw SocketError("Unable to configure SSL certificate file.");
    }

    if (SSL_CTX_use_PrivateKey_file(sslCtx, tls->keyFile.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
        throw SocketError("Unable to configure SSL key file.");
    }
}

void HTTPServ::ServerSocket::cleanTLS() {
    SSL_CTX_free(sslCtx);
    EVP_cleanup();
}

int HTTPServ::ServerSocket::getPort() {
    return port;
}

HTTPServ::SocketError::SocketError(const std::string &message) : message(message) {}

const std::string &HTTPServ::SocketError::getMessage() const {
    return message;
}
