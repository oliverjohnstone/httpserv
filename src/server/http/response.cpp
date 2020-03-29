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
