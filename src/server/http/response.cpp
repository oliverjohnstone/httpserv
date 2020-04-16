//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/response.h>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <string>

using namespace std;

HTTPServ::Response::Response(io::stream<OutSocketStream> &stream, int maxRequests, int socketTimeout) :
    stream(stream), maxRequests(maxRequests), socketTimeout(socketTimeout) {
}

HTTPServ::Response& HTTPServ::Response::status(HTTP::STATUS code) {
    statusCode = code;
    return *this;
}

void HTTPServ::Response::sendHeaders() {
    if (!headersSent) {
        stream << (httpVersion ?: HTTP::VERSION_1_1) << " " << statusCode << " " << HTTP::STATUS_TEXT[statusCode] << HTTP::PROTO_ENDL;

        for (auto [name, value] : headers) {
            stream << name << ": " << value << HTTP::PROTO_ENDL;
        }

        stream << HTTP::PROTO_ENDL;
    }

    headersSent = true;
}

HTTPServ::Response& HTTPServ::Response::end(const string &body) {
    if (ended) {
        return *this;
    }

    header("Content-Length", to_string(body.length()));
    sendHeaders();
    stream << body;

    flush();
    ended = true;
    return *this;
}

HTTPServ::Response& HTTPServ::Response::end(const json& body) {
    return header("Content-Type", "application/json").end(body.dump());
}

HTTPServ::Response& HTTPServ::Response::end(const char * body) {
    return end(string(body));
}

HTTPServ::Response& HTTPServ::Response::header(const string& name, const string& value) {
    if (headersSent) {
        throw runtime_error("Headers already sent");
    }
    headers[boost::algorithm::to_lower_copy(name)] = value;
    return *this;
}

void HTTPServ::Response::syncWith(HTTPServ::Request& request) {
    httpVersion = request.getHTTPVersion();

    if (request.shouldClose()) {
        header("Connection", "close");
    } else {
        header("Connection", "keep-alive");
        header("Keep-Alive", "timeout="s + to_string(socketTimeout) + ", max="s + to_string(maxRequests));
    }
}

HTTPServ::HTTP::STATUS HTTPServ::Response::getStatus() {
    return statusCode;
}

void HTTPServ::Response::flush() {
    stream.flush();
}
