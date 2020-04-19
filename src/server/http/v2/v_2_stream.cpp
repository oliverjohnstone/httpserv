//
// Created by Oliver Johnstone on 19/04/2020.
//

#include "v_2_stream.h"

HTTPServ::V2InputStream::V2InputStream(int inFd) : socketFd(inFd) {}

std::streamsize HTTPServ::V2InputStream::read(char *buf, std::streamsize num) {
    return ::read(socketFd, buf, num);
}

void HTTPServ::V2InputStream::close() {
    ::close(socketFd);
}

HTTPServ::V2OutputStream::V2OutputStream(int outFd) : socketFd(outFd) {}

std::streamsize HTTPServ::V2OutputStream::write(const char *s, std::streamsize n) {
    return ::write(socketFd, s, n);
}

void HTTPServ::V2OutputStream::close() {
    ::close(socketFd);
}
