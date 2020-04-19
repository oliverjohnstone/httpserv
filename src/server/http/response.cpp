//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/response.h>
#include <boost/algorithm/string.hpp>
#include <string>

using namespace std;

HTTPServ::Response::Response(HTTPServ::HTTPVersion *httpImpl) : httpImpl(httpImpl) {
}

HTTPServ::Response& HTTPServ::Response::status(HTTP::STATUS code) {
    statusCode = code;
    return *this;
}

void HTTPServ::Response::sendHeaders() {
    if (!headersSent) {
        httpImpl->writeStatusHeader(statusCode);
        httpImpl->writeHeaders(headers);
    }

    headersSent = true;
}

HTTPServ::Response& HTTPServ::Response::end(const string &body) {
    if (ended) {
        return *this;
    }

    header("Content-Length", to_string(body.length()));
    sendHeaders();
    httpImpl->writeBody(body);

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
    // TODO - Implement at proto level
//    httpVersion = request.getHTTPVersion();

//    if (request.shouldClose()) {
//        header("Connection", "close");
//    } else {
//        header("Connection", "keep-alive");
//        header("Keep-Alive", "timeout="s + to_string(socketTimeout) + ", max="s + to_string(maxRequests));
//    }
}

HTTPServ::HTTP::STATUS HTTPServ::Response::getStatus() {
    return statusCode;
}

void HTTPServ::Response::flush() {
    httpImpl->flush();
}
