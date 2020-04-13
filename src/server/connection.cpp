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
    request->parseHeaders();
    response->syncWith(request);
}

HTTPServ::Request *HTTPServ::Connection::getRequest() const {
    return request;
}

HTTPServ::Response *HTTPServ::Connection::getResponse() const {
    return response;
}

void HTTPServ::Connection::handleRequest(const std::vector<HTTPServ::Router *>& routers) {
    try {
        auto found = false;
        parseRequestHeaders();

        for (auto router : routers) {
            found |= router->getHandler(request->getVerb(), request->getUri())(request, response);
        }

        if (!found) {
            response
            ->status(HTTP::STATUS::NOT_FOUND)
            ->header("Content-Type", "text/plain")
            ->end("Not Found");
        }
    } catch (HTTPError& e) {
        request->log()->warn(e.getMessage().c_str());
        response
        ->status(e.getCode())
        ->header("Content-Type", "text/plain")
        ->end(e.getMessage());
    } catch (std::exception& e) {
        std::stringstream ss;
        ss << "Exception of type (" << e.what() << ") thrown whilst handling request.";
        request->log()->error(ss.str().c_str());
        response
        ->status(HTTP::STATUS::INTERNAL_SERVER_ERROR)
        ->header("Content-Type", "text/plain")
        ->end("Internal Server Error");
    } catch (...) {
        request->log()->error("Exception of unknown type thrown whilst handling request");
        response
        ->status(HTTP::STATUS::INTERNAL_SERVER_ERROR)
        ->header("Content-Type", "text/plain")
        ->end("Internal Server Error");
    }
}
