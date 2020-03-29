//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_CONNECTION_H
#define HTTPSERV_CONNECTION_H

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
            void finish();
            void reject();

        void parseRequestHeaders();
    };
}

#endif //HTTPSERV_CONNECTION_H
