//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/connection.h>
#include <server/error.h>

HTTPServ::Connection::Connection(HTTPServ::Request *request, HTTPServ::Response *response) : request(request), response(response) {
}

HTTPServ::Connection::~Connection() {
    response->close();
    delete request;
    delete response;
}

void HTTPServ::Connection::reject() {
    response->status(HTTP::STATUS::SERVICE_UNAVAILABLE)->end("Server Unavailable");
}

void HTTPServ::Connection::parseRequestHeaders() {
    try {
        request->parseHeaders();
        response->syncWith(request);
    } catch (HTTPError& e) {
        response->status(e.getCode())->end(e.getMessage());
        return;
    }
}

HTTPServ::Request *HTTPServ::Connection::getRequest() const {
    return request;
}

HTTPServ::Response *HTTPServ::Connection::getResponse() const {
    return response;
}
