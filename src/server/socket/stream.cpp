//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/stream.h>

std::streamsize HTTPServ::InSocketStream::read(char *buf, std::streamsize num) {
    int result;
    if (ssl) {
        result = SSL_read(ssl, buf, num);
    } else {
        result = ::read(socketFd, buf, num);
    }

    if (result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        timedOut = true;
    }

    return result;
}

HTTPServ::InSocketStream::InSocketStream(int socketFd) : socketFd(socketFd) {}

HTTPServ::InSocketStream::InSocketStream(int socketFd, SSL *ssl, ALPN alpn) : ssl(ssl), socketFd(socketFd), alpn(alpn) {
}

void HTTPServ::InSocketStream::close() {
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    ::close(socketFd);
}

bool HTTPServ::InSocketStream::didTimeout() {
    return timedOut;
}

bool HTTPServ::InSocketStream::isH2Socket() {
    return alpn == ALPN::H2;
}

HTTPServ::OutSocketStream::OutSocketStream(int socketFd) : socketFd(socketFd) {}
HTTPServ::OutSocketStream::OutSocketStream(int socketFd, SSL *ssl) : ssl(ssl), socketFd(socketFd) {
}

std::streamsize HTTPServ::OutSocketStream::write(const char* buf, std::streamsize num) {
    if (ssl) {
        return SSL_write(ssl, buf, num);
    } else {
        return ::write(socketFd, buf, num);
    }
}

void HTTPServ::OutSocketStream::close() {
    ::close(socketFd);
}