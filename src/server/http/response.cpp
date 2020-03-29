//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/response.h>

HTTPServ::Response::Response(io::stream<OutSocketStream> *stream) : stream(stream) {}

HTTPServ::Response::~Response() {
    delete stream;
}

void HTTPServ::Response::flush() {
    // TODO - Flush anything buffered out to the socket
}

void HTTPServ::Response::close() {
    flush();
    stream->close();
}

HTTPServ::Response* HTTPServ::Response::status(int code) {
    // TODO - Send status code
    return this;
}

HTTPServ::Response* HTTPServ::Response::end(const std::string &body) {
    // TODO - Send body
    return this;
}
