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

    auto text = HTTP::STATUS_TEXT[code];
    if (!text) {
        // TODO - Check this mapping?
        // TODO - use defaults
    }

    return this;
}

void HTTPServ::Response::sendHeaders() {
    if (!headersSent) {
        *stream << httpVersion << " " << statusCode << " " << HTTP::STATUS_TEXT[statusCode] << "\r\n";

        for (auto [name, value] : headers) {
            *stream << name << ": " << value << "\r\n";
        }

        *stream << "Connection: Closed\r\n" // TODO - Remove this once connections can remain open
            << "\r\n";
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
