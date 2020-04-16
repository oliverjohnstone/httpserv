//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_CONNECTION_H
#define HTTPSERV_CONNECTION_H

#include <router/router.h>
#include "request.h"
#include "response.h"

namespace HTTPServ {
    class Connection {
        private:
            io::stream<InSocketStream>& in;
            io::stream<OutSocketStream>& out;
            int maxRequests;
            int socketTimeout;

            void logRequestStart(Logger &logger);
            void logRequestEnd(Logger &logger, Request &req, Response &res);

        public:
            Connection(io::stream<InSocketStream> &in, io::stream<OutSocketStream> &out, int maxRequests, int socketTimeout);
            void handleRequests(const std::vector<std::reference_wrapper<HTTPServ::Router>> &routers, Logger &parentLogger);
    };
}

#endif //HTTPSERV_CONNECTION_H
