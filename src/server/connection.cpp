//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/connection.h>
#include <server/error.h>
#include <sstream>

HTTPServ::Connection::Connection(HTTPServ::Request *request, HTTPServ::Response *response) : request(request), response(response) {
}

HTTPServ::Connection::~Connection() {
    std::stringstream ss;
    ss << "[" << request->getVerbAsString() << " " << response->getStatus() << "] " << request->getUri();
    request->log()->info(ss.str().c_str());

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

void HTTPServ::Connection::handleRequest(const std::vector<HTTPServ::Router *>& routers) {
    parseRequestHeaders();

    auto req = getRequest();
    auto res = getResponse();

    try {
        auto found = false;

        for (auto router : routers) {
            found |= router->getHandler(req->getVerb(), req->getUri())(req, res);
        }

        if (!found) {
            res
            ->status(HTTP::STATUS::NOT_FOUND)
            ->header("Content-Type", "text/plain")
            ->end("Not Found");
        }
    } catch (std::exception& e) {
        // TODO - Perhaps offload to a middleware
        req->log()->error(e.what());
        res
        ->status(HTTP::STATUS::INTERNAL_SERVER_ERROR)
        ->header("Content-Type", "text/plain")
        ->end("Internal Server Error");
    }
}
