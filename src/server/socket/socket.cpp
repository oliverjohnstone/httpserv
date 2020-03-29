//
// Created by Oliver Johnstone on 24/03/2020.
//

#include <server/socket.h>

HTTPServ::ServerSocket::ServerSocket(int port, int maxConnections) : port(port), maxConnections(maxConnections) {
}

void HTTPServ::ServerSocket::listen() {
    auto opt = 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock) {
        throw "Unable to create new socket";
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        throw "Unable to set socket options";
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw "Unable to bind to port";
    }

    if (::listen(sock, maxConnections) < 0) {
        throw "Unable to listen";
    }
}

std::tuple<io::stream<HTTPServ::InSocketStream>*, io::stream<HTTPServ::OutSocketStream>*> HTTPServ::ServerSocket::waitForClientConnection() {
    auto addressLength = sizeof(address);
    auto client_sock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addressLength);
    if (client_sock < 0) {
        throw "Invalid client socket";
    }

    auto in = new io::stream<InSocketStream>(client_sock);
    auto out = new io::stream<OutSocketStream>(client_sock);

    return {in, out};
}

void HTTPServ::ServerSocket::close() {
    ::close(sock);
}

HTTPServ::ServerSocket::~ServerSocket() {
    close();
}
