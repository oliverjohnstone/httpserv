//
// Created by Oliver Johnstone on 23/03/2020.
//

#ifndef HTTPSERV_SERVER_H
#define HTTPSERV_SERVER_H

#include "logger/logger.h"
#include "socket.h"
#include "tls.h"
#include <server/connection.h>
#include <future>
#include <vector>
#include <router/router.h>

namespace HTTPServ {
    class Server {
        private:
            Logger& logger;
            ServerSocket* socket;
            std::atomic_bool running;
            std::vector<std::future<void>> connectionHandlers;
            std::vector<std::reference_wrapper<HTTPServ::Router>> routers;

            int socketTimeout = 10;
            int maxRequestPerSocket = 100;

            int run_async();
            void handle_request(io::stream<InSocketStream> *in, io::stream<OutSocketStream> *out);

        public:
            Server(int port, Logger &logger);
            Server(int port, Logger &logger, const TLS::Config *tlsConfig);
            virtual ~Server();
            int run();
            void stop();

            Server& attachRouter(HTTPServ::Router &router);
    };
}

#endif //HTTPSERV_SERVER_H
