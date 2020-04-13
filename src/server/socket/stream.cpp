//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/stream.h>

std::streamsize HTTPServ::InSocketStream::read(char *buf, std::streamsize num) {
    if (ssl) {
        return SSL_read(ssl, buf, num);
    } else {
        return ::read(socketFd, buf, num);
    }
}

HTTPServ::InSocketStream::InSocketStream(int socketFd) : socketFd(socketFd) {}

HTTPServ::InSocketStream::InSocketStream(int socketFd, SSL *ssl) : ssl(ssl), socketFd(socketFd) {}

void HTTPServ::InSocketStream::close() {
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    ::close(socketFd);
}

HTTPServ::OutSocketStream::OutSocketStream(int socketFd) : socketFd(socketFd) {}
HTTPServ::OutSocketStream::OutSocketStream(int socketFd, SSL *ssl) : ssl(ssl), socketFd(socketFd) {}

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