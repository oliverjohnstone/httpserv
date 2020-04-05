//
// Created by Oliver Johnstone on 04/04/2020.
//

#ifndef HTTPSERV_ROUTER_H
#define HTTPSERV_ROUTER_H

#include <string>
#include <server/request.h>
#include <server/response.h>

namespace HTTPServ {
    typedef std::function<void(Request *, Response *)> Route;

    class Router {
        private:
            typedef struct {
                const char *path; // TODO - Replace with regex handlers
                HTTP::VERB verb;
            } HandlerDescription;
            typedef std::vector<std::pair<HandlerDescription, std::vector<Route>>> Handles;
            typedef std::vector<Route> FilteredHandles;

            Handles useHandlers;
            Handles handlers;

            void handle(FilteredHandles* useChain, FilteredHandles* verbChain, Request *req, Response *res);

        public:
            Router();
            Router* use(const char *path, Route handler);
            Route getHandler(HTTPServ::HTTP::VERB verb, std::string &path);

            template<class FnPtr, class ClassT>
            static Route boundHandler(FnPtr fnPtr, ClassT &instance) {
                return std::bind(fnPtr, instance, std::placeholders::_1, std::placeholders::_2);
            }
    };
}

#endif //HTTPSERV_ROUTER_H
