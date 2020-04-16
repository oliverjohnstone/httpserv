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

using json = nlohmann::json;
namespace io = boost::iostreams;

namespace HTTPServ {
    class Request {
        private:
            io::stream<InSocketStream>& stream;
            std::unordered_map<std::string, std::string> headers;
            HTTP::VERB verb = HTTP::VERB::NONE;
            std::string uri;
            const char* httpVersion;
            Logger& logger;
            json context;
            PathMatcher::ArgResults* args;
            std::unordered_map<std::string, std::string> query;
            int maxRequests;
            int socketTimeout;

            void parseRequestLine();
            void getLine(std::string &out);
            void parseQueryString(const std::string &queryString);

        public:
            Request(io::stream<InSocketStream> &stream, Logger &logger);
            void parseHeaders();
            Logger& log();
            std::string& getUri();
            const char *getHTTPVersion();
            HTTP::VERB getVerb();
            json& getContext();
            void setArgs(PathMatcher::ArgResults *reqArgs);
            std::string getArg(const std::string &name);
            std::string getArg(int index);
            std::string getQuery(const std::string &name);
            std::string getVerbAsString();
            bool shouldClose();
    };
}

#endif //HTTPSERV_REQUEST_H
