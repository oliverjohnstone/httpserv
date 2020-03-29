//
// Created by Oliver Johnstone on 23/03/2020.
//

#include <server/server.h>
#include <logger/logger.h>
#include <iostream>

int main() {
    HTTPServ::Logger logger(std::cout);
    HTTPServ::Server server(8080, logger);

    return server.run();
}