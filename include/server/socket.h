//
// Created by Oliver Johnstone on 24/03/2020.
//

#ifndef HTTPSERV_SOCKET_H
#define HTTPSERV_SOCKET_H

#include <tuple>
#include <boost/iostreams/stream.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include "stream.h"

namespace io = boost::iostreams;

namespace HTTPServ {
    class SocketError : public std::exception {
        private:
            std::string message;

        public:
            explicit SocketError(const std::string &message);
            [[nodiscard]] const std::string &getMessage() const;
    };

    class ServerSocket {
        private:
            int port, maxConnections, sock;
            struct sockaddr_in address;

        public:
            ServerSocket(int port, int maxConnections);
            virtual ~ServerSocket();
            void listen();
            void close();
            std::tuple<io::stream<InSocketStream>*, io::stream<OutSocketStream>*> waitForClientConnection();
    };
}

#endif //HTTPSERV_SOCKET_H
