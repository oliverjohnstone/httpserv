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

HTTPServ::Response* HTTPServ::Response::status(HTTP::STATUS code) {
    statusCode = code;
    return this;
}

void HTTPServ::Response::sendHeaders() {
    if (!headersSent) {
        *stream << httpVersion << " " << statusCode << " " << HTTP::STATUS_TEXT[statusCode] << HTTP::PROTO_ENDL;

        for (auto [name, value] : headers) {
            *stream << name << ": " << value << HTTP::PROTO_ENDL;
        }

        *stream << "Connection: Closed" << HTTP::PROTO_ENDL // TODO - Remove this once connections can remain open
            << HTTP::PROTO_ENDL;
    }

    headersSent = true;
}

HTTPServ::Response* HTTPServ::Response::end(const std::string &body) {
    sendHeaders();
    *stream << body;
    return this;
}

HTTPServ::Response* HTTPServ::Response::header(const std::string& name, std::string value) {
    if (headersSent) {
        throw std::runtime_error("Headers already sent");
    }
    headers[name] = value;
    return this;
}

void HTTPServ::Response::syncWith(HTTPServ::Request *request) {
    httpVersion = request->getHTTPVersion();
    // TODO - Sync should close connections etc
}