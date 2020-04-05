//
// Created by Oliver Johnstone on 04/04/2020.
//

#include <router/router.h>

template <typename HandlerT>
HTTPServ::Router<HandlerT>::Router() {}

template <typename HandlerT>
HTTPServ::Router<HandlerT>* HTTPServ::Router<HandlerT>::use(const char *path, HandlerT handler) {
    useHandlers.push_back({{.path = path}, {handler}});
    return this;
}

template<typename HandlerT>
HTTPServ::RouteHandler HTTPServ::Router<HandlerT>::getHandler(HTTPServ::HTTP::VERB verb, std::string &path) {
    auto routeMatches = [verb, path](HandlerDescription &handlerDescription) -> bool {
        auto pathMatches = handlerDescription.path == path;
        auto verbIsNone = handlerDescription.verb == HTTP::VERB::NONE;
        auto verbMatches = handlerDescription.verb == verb;

        return pathMatches && (verbIsNone || verbMatches);
    };

    auto find = [routeMatches](HandlerCollection &collection) -> std::vector<HandlerT>* {
        for (auto &[handlerDescription, useHandlerChain] : collection) {
            if (routeMatches(handlerDescription)) {
                return &useHandlerChain;
            }
        }
        return nullptr;
    };

    return [this, find](Request * req, Response *res) {
        // Bind handle function with filtered use and verb handler chains
        handle(find(useHandlers), find(handlers), req, res);
    };
}

template<typename HandlerT>
void HTTPServ::Router<HandlerT>::handle(std::vector<HandlerT> *useChain, std::vector<HandlerT> *verbChain,
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

// Ensure templates exist for the different forms of handler
template class HTTPServ::Router<HTTPServ::RouteHandler>;
