//
// Created by Oliver Johnstone on 04/04/2020.
//

#include <router/router.h>

HTTPServ::Router::Router() {}

std::function<bool(HTTPServ::Request *, HTTPServ::Response *)> HTTPServ::Router::getHandler(
        HTTPServ::HTTP::VERB verb, std::string &path) {

    auto args = new PathMatcher::ArgResults;

    auto routeMatches = [verb, path, args](HandlerDescription &handlerDescription) -> bool {
        auto pathMatches = handlerDescription.path->matches(path, *args);
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

    return std::bind(
            &Router::handle,
            this,
            findFn(useHandlers),
            findFn(handlers),
            args,
            std::placeholders::_1,
            std::placeholders::_2
    );
}

bool HTTPServ::Router::handle(FilteredHandles* useChain, FilteredHandles* verbChain, PathMatcher::ArgResults* args,
                              HTTPServ::Request *req, HTTPServ::Response *res) {

    auto matched = false;

    for (auto chain : {useChain, verbChain}) {
        if (chain == nullptr) {
            continue;
        }

        if (chain->size()) {
            req->setArgs(args);
            matched = true;
        }

        for (auto&& handler : *chain) {
            handler(req, res);
        }
    }

    return matched;
}

HTTPServ::Router* HTTPServ::Router::use(const char *path, FilteredHandles &handleChain) {
    useHandlers.push_back({{.path= new PathMatcher::Matcher(path)}, handleChain});
    return this;
}

HTTPServ::Router* HTTPServ::Router::verb(const char *path, HTTPServ::HTTP::VERB verb, HTTPServ::Router::FilteredHandles &handleChain) {
    handlers.push_back({{.path= new PathMatcher::Matcher(path), .verb=verb}, handleChain});
    return this;
}

HTTPServ::Router::~Router() {
    for (auto &collection : {handlers, useHandlers}) {
        for (auto &[handler, routes] : collection) {
            delete handler.path;
        }
    }
}
