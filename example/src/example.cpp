//
// Created by Oliver Johnstone on 23/03/2020.
//

#include <server/server.h>
#include <logger/logger.h>
#include <router/router.h>
#include <iostream>

int main() {
    HTTPServ::Logger logger(std::cout);
    HTTPServ::Server server(8080, logger);
    HTTPServ::Router<HTTPServ::RouteHandler> router;

    router.use("/test", [](HTTPServ::Request *req, HTTPServ::Response *res){
        std::cout << "Test route" << std::endl;
    });

    std::string test = "/test";
    std::string test2 = "/test2";

    router.getHandler(HTTPServ::HTTP::VERB::NONE, test)(nullptr, nullptr);
    router.getHandler(HTTPServ::HTTP::VERB::NONE, test2)(nullptr, nullptr);

    return server.run();
}