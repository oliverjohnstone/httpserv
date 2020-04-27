//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_REQUEST_H
#define HTTPSERV_REQUEST_H

#include <boost/iostreams/stream.hpp>
#include <map>
#include <logger/logger.h>
#include <nlohmann/json.hpp>
#include <path_matcher/path_matcher.h>
#include "stream.h"
#include "http.h"
#include "../../src/server/http/version.h"

using json = nlohmann::json;
namespace io = boost::iostreams;

namespace HTTPServ {
    class Request {
        private:
            HTTPVersion* httpImpl = nullptr;
            Logger& logger;
            json context;
            PathMatcher::ArgResults* args;

        public:
            Request(HTTPVersion *httpImpl, Logger &logger);
            Logger& log();
            std::string& getUri();
            HTTP::VERB getVerb();
            json& getContext();
            void setArgs(PathMatcher::ArgResults *reqArgs);
            std::string getArg(const std::string &name);
            std::string getArg(int index);
            std::string getVerbAsString();
            std::string& getQuery(const std::string& name);
            bool shouldClose();
    };
}

#endif //HTTPSERV_REQUEST_H
