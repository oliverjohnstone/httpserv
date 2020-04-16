//
// Created by Oliver Johnstone on 24/03/2020.
//

#ifndef HTTPSERV_SOCKET_H
#define HTTPSERV_SOCKET_H

#include <tuple>
#include <boost/iostreams/stream.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include "stream.h"
#include "tls.h"

namespace io = boost::iostreams;

namespace HTTPServ {
    class SocketError : public std::exception {
        private:
            std::string message;

        public:
            explicit SocketError(const std::string &message);
            [[nodiscard]] const std::string &getMessage() const;
    };

    class SocketTimeout : public std::exception {};

    class ServerSocket {
        typedef std::tuple<io::stream<HTTPServ::InSocketStream>*, io::stream<HTTPServ::OutSocketStream>*> SocketStreams;

        private:
            int port, maxConnections, sock;
            struct sockaddr_in address;
            const TLS::Config* tls = nullptr;
            SSL_CTX* sslCtx = nullptr;

            void initTLS();
            void cleanTLS();

        public:
            ServerSocket(int port, int maxConnections);
            ServerSocket(int port, int maxConnections, const TLS::Config* tlsConfig);
            virtual ~ServerSocket();
            void listen();
            void close();
            SocketStreams waitForClientConnection(int socketTimeout);
            int getPort();
    };
}

#endif //HTTPSERV_SOCKET_H
