//
// Created by Oliver Johnstone on 04/04/2020.
//

#ifndef HTTPSERV_ROUTER_H
#define HTTPSERV_ROUTER_H

#include <string>
#include <server/request.h>
#include <server/response.h>
#include <path_matcher/path_matcher.h>

namespace HTTPServ {
    typedef std::function<void(Request *, Response *)> Route;

    class Router {
        private:
            typedef struct {
                PathMatcher::Matcher *path;
                HTTP::VERB verb;
            } HandlerDescription;
            typedef std::vector<Route> FilteredHandles;
            typedef std::vector<std::pair<HandlerDescription, FilteredHandles>> Handles;

            Handles useHandlers;
            Handles handlers;

            bool handle(FilteredHandles* useChain, FilteredHandles* verbChain, PathMatcher::ArgResults* args, Request *req, Response *res);
            Router* use(const char *path, FilteredHandles &handleChain);
            Router* verb(const char *path, HTTP::VERB verb, FilteredHandles &handleChain);

        public:
            Router();

        virtual ~Router();

        template<typename... Handlers>
            Router* use(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return use(path, handlerFns);
            }

            template<typename... Handlers>
            Router* get(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::GET, handlerFns);
            }

            template<typename... Handlers>
            Router* post(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::POST, handlerFns);
            }

            template<typename... Handlers>
            Router* put(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::PUT, handlerFns);
            }

            template<typename... Handlers>
            Router* del(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::DELETE, handlerFns);
            }

            template<typename... Handlers>
            Router* options(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::OPTIONS, handlerFns);
            }

            template<typename... Handlers>
            Router* head(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::HEAD, handlerFns);
            }

            template<typename... Handlers>
            Router* patch(const char *path, Handlers&&... handlerArgs) {
                FilteredHandles handlerFns;
                (handlerFns.push_back(handlerArgs), ...);
                return verb(path, HTTP::VERB::PATCH, handlerFns);
            }

            std::function<bool(Request *, Response *)> getHandler(HTTPServ::HTTP::VERB verb, std::string &path);

            template<class FnPtr, class ClassT>
            static Route boundHandler(FnPtr fnPtr, ClassT &instance) {
                return std::bind(fnPtr, instance, std::placeholders::_1, std::placeholders::_2);
            }
    };
}

#endif //HTTPSERV_ROUTER_H
