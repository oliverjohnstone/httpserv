//
// Created by Oliver Johnstone on 04/04/2020.
//

#ifndef HTTPSERV_ROUTER_H
#define HTTPSERV_ROUTER_H

#include <string>
#include <server/request.h>
#include <server/response.h>

namespace HTTPServ {
    typedef std::function<void(Request *, Response *)> RouteHandler;

    template <typename HandlerT>
    class Router {
        private:
            typedef struct {
                const char *path; // TODO - Replace with regex handlers
                HTTP::VERB verb;
            } HandlerDescription;
            typedef std::vector<std::pair<HandlerDescription, std::vector<HandlerT>>> HandlerCollection;

            HandlerCollection useHandlers;
            HandlerCollection handlers;

            void handle(std::vector<HandlerT> *useChain, std::vector<HandlerT> *verbChain, Request *req, Response *res);

        public:
            Router();
            Router* use(const char *path, HandlerT handler);
            RouteHandler getHandler(HTTPServ::HTTP::VERB verb, std::string &path);
    };
}

#endif //HTTPSERV_ROUTER_H
