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
            Request *request;
            Response *response;

        public:
            Connection(Request *request, Response *response);
            virtual ~Connection();
            void reject();
            void parseRequestHeaders();
            [[nodiscard]] Request *getRequest() const;
            [[nodiscard]] Response *getResponse() const;
            void handleRequest(const std::vector<HTTPServ::Router *>& routers);
    };
}

#endif //HTTPSERV_CONNECTION_H
