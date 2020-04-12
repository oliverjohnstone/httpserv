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

    auto isFinished = [](std::future<void> &conn){
        std::chrono::milliseconds timeout(100);
        return conn.wait_for(timeout) == std::future_status::ready;
    };

    while (running) {
        connectionHandlers.erase(
                std::remove_if(connectionHandlers.begin(),connectionHandlers.end(), isFinished),
                connectionHandlers.end()
        );

        // TODO - Support timeout etc
        // TODO - Do any server processing / clean up etc here

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    return futureResult.get();
}

int HTTPServ::Server::run_async() {
    running = true;

    while(running) {
        try {
            auto [in, out] = socket->waitForClientConnection();
            auto conn = new Connection(new Request(in, logger), new Response(out));

            if (running) {
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
    conn->handleRequest(routers);
    delete conn;
}

HTTPServ::Server* HTTPServ::Server::attachRouter(HTTPServ::Router *router) {
    routers.push_back(router);
    return this;
}