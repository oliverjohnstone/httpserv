//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/connection.h>

HTTPServ::Connection::Connection(HTTPServ::Request *request, HTTPServ::Response *response) : request(request), response(response) {}

HTTPServ::Connection::~Connection() {
    delete request;
    delete response;
}

void HTTPServ::Connection::finish() {
    response->close();
}

void HTTPServ::Connection::reject() {
    // TODO - Respond with some sensible http status code
    finish();
}

void HTTPServ::Connection::parseRequestHeaders() {
    try {
        request->parseHeaders();
    } catch (int e) {
        throw e;
        // TODO - Respond with http status code specified in e
    }
    response->close();
}
