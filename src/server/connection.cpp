//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/connection.h>
#include <server/error.h>

HTTPServ::Connection::Connection(HTTPServ::Request *request, HTTPServ::Response *response) : request(request), response(response) {
}

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
    } catch (HTTPError& e) {
        response->status(e.getCode())->end(e.getMessage());
        return;
    }

    response->status(HTTP::STATUS::OK)->header("Content-type", "application/json")->end(R"({"hello": "world"})");
}
