//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/stream.h>

std::streamsize HTTPServ::InSocketStream::read(char *buf, std::streamsize num) {
    return ::read(socketFd, buf, num);
}

HTTPServ::InSocketStream::InSocketStream(int socketFd) : socketFd(socketFd) {}

void HTTPServ::InSocketStream::close() {
    ::close(socketFd);
}

HTTPServ::OutSocketStream::OutSocketStream(int socketFd) : socketFd(socketFd) {}

std::streamsize HTTPServ::OutSocketStream::write(const char* buf, std::streamsize num) {
    return ::write(socketFd, buf, num);
}

void HTTPServ::OutSocketStream::close() {
    ::close(socketFd);
}