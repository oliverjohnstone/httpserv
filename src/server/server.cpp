//
// Created by Oliver Johnstone on 23/03/2020.
//

#include <server/server.h>
#include <server/socket.h>
#include <future>
#include <server/connection.h>

using namespace std;

HTTPServ::Server::Server(int port, Logger& logger) : port(port), logger(logger), running(false), socket(nullptr) {
}

int HTTPServ::Server::run() {
    socket = new ServerSocket(port, 50);
    try {
        socket->listen();
    } catch (SocketError& e) {
        logger.error(e.getMessage().c_str());
        return 1;
    }

    auto futureResult = async(&Server::run_async, this);

    logger.info(("Started on http://localhost:" + to_string(port)).c_str());

    // TODO - Do any server processing / clean up etc here

    return futureResult.get();
}

int HTTPServ::Server::run_async() {
    running = true;

    while(running) {
        try {
            auto [in, out] = socket->waitForClientConnection();
            auto conn = new Connection(new Request(in, logger), new Response(out));

            if (running) {
                // TODO - Remove from connection handlers once finished
                // TODO - This will probably be more efficient with a threadpool
                connectionHandlers.push_back(async(&Server::handle_request, this, conn));
            } else {
                conn->reject();
                delete conn;
            }
        } catch (SocketError& e) {
            logger.error(e.getMessage().c_str());
            continue; // TODO - perhaps the server socket has failed and we should restart?
        }
    }

    return 0;
}

HTTPServ::Server::~Server() {
    delete socket;
}

void HTTPServ::Server::stop() {
    running = false;
}

void HTTPServ::Server::handle_request(HTTPServ::Connection *conn) {
    conn->parseRequestHeaders();

    // TODO - Do any more pre request handling processing here

    auto req = conn->getRequest();
    auto res = conn->getResponse();

    for (auto router : routers) {
        // TODO - try/catch and throw server error
        router->getHandler(req->getVerb(), req->getUri())(req, res);
    }

    delete conn;
}

HTTPServ::Server* HTTPServ::Server::attachRoutes(HTTPServ::Router *router) {
    routers.push_back(router);
    return this;
}