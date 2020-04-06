//
// Created by Oliver Johnstone on 04/04/2020.
//

#include <router/router.h>

HTTPServ::Router::Router() {}

HTTPServ::Route HTTPServ::Router::getHandler(HTTPServ::HTTP::VERB verb, std::string &path) {
    auto routeMatches = [verb, path](HandlerDescription &handlerDescription) -> bool {
        auto pathMatches = handlerDescription.path == path;
        auto verbIsNone = handlerDescription.verb == HTTP::VERB::NONE;
        auto verbMatches = handlerDescription.verb == verb;

        return pathMatches && (verbIsNone || verbMatches);
    };

    auto findFn = [routeMatches](Handles &collection) -> std::vector<Route>* {
        for (auto &[handlerDescription, useHandlerChain] : collection) {
            if (routeMatches(handlerDescription)) {
                return &useHandlerChain;
            }
        }
        return nullptr;
    };

    return std::bind(&Router::handle, this, findFn(useHandlers), findFn(handlers), std::placeholders::_1, std::placeholders::_2);
}

void HTTPServ::Router::handle(FilteredHandles* useChain, FilteredHandles* verbChain,
        HTTPServ::Request *req, HTTPServ::Response *res) {

    for (auto chain : {useChain, verbChain}) {
        if (chain == nullptr) {
            continue;
        }

        // TODO - Error handling etc

        for (auto&& handler : *chain) {
            handler(req, res);
        }
    }
}

HTTPServ::Router* HTTPServ::Router::use(const char *path, FilteredHandles &handleChain) {
    useHandlers.push_back({{.path = path}, handleChain});
    return this;
}

HTTPServ::Router* HTTPServ::Router::verb(const char *path, HTTPServ::HTTP::VERB verb, HTTPServ::Router::FilteredHandles &handleChain) {
    handlers.push_back({{.path = path, .verb = verb}, handleChain});
    return this;
}
