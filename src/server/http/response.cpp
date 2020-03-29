//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/response.h>
#include <sstream>

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
    statusCode = code;
    return this;
}

void HTTPServ::Response::sendHeaders() {
    *stream <<
        httpVersion << " " << statusCode << " OK\r\n"
        << "Connection: Closed\r\n"
        << "\r\n";
}

HTTPServ::Response* HTTPServ::Response::end(const std::string &body) {
    sendHeaders();
    *stream << body;
    return this;
}
