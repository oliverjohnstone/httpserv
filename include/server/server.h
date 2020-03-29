//
// Created by Oliver Johnstone on 23/03/2020.
//

#ifndef HTTPSERV_SERVER_H
#define HTTPSERV_SERVER_H

#include "logger/logger.h"
#include "socket.h"
#include <server/connection.h>
#include <future>
#include <vector>

namespace HTTPServ {
    class Server {
        private:
            int port;
            Logger& logger;
            ServerSocket* socket;
            std::atomic_bool running;
            std::vector<std::future<void>> connectionHandlers;

            int run_async();
            void handle_request(Connection *conn);

        public:
            Server(int port, Logger& logger);
            virtual ~Server();
            int run();
            void stop();
    };
}

#endif //HTTPSERV_SERVER_H
