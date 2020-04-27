//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/request.h>
#include <server/error.h>

using namespace std;

HTTPServ::Request::Request(HTTPVersion *httpImpl, Logger &logger) : httpImpl(httpImpl), args(nullptr), logger(logger) {
}

HTTPServ::Logger& HTTPServ::Request::log() {
    return logger;
}

std::string HTTPServ::Request::getVerbAsString() {
    auto str = HTTP::VERB_TEXT[httpImpl->getVerb()];
    return str ?: "INVALID";
}

std::string& HTTPServ::Request::getQuery(const std::string& name) {
    return httpImpl->getQuery(name);
}

std::string& HTTPServ::Request::getUri() {
    return httpImpl->getUri();
}

HTTPServ::HTTP::VERB HTTPServ::Request::getVerb() {
    return httpImpl->getVerb();
}

json& HTTPServ::Request::getContext() {
    return context;
}

void HTTPServ::Request::setArgs(PathMatcher::ArgResults* reqArgs) {
    args = reqArgs;
}

std::string HTTPServ::Request::getArg(const std::string& name) {
    if (!args) {
        throw std::logic_error("Request arguments have not been set.");
    }

    auto str = (*args)[name];

    if (str.empty()) {
        throw std::invalid_argument("There is no argument provided for: "s + name + "."s);
    }

    return str;
}

string HTTPServ::Request::getArg(int index) {
    return getArg("captureGroup_"s + to_string(index));
}

bool HTTPServ::Request::shouldClose() {
    return httpImpl->getHeader("connection") == "close";
}
