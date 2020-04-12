//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_REQUEST_H
#define HTTPSERV_REQUEST_H

#include <boost/iostreams/stream.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
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
            io::stream<InSocketStream> *stream;
            std::map<std::string, std::string> headers;
            HTTP::VERB verb = HTTP::VERB::NONE;
            std::string uri;
            const char * httpVersion;
            Logger* logger;
            boost::uuids::uuid id;
            json context;
            PathMatcher::ArgResults* args;

            void parseRequestLine();
            static void getLine(std::istream* is, std::string& out);

        public:
            Request(io::stream<InSocketStream>* stream, Logger& logger);
            virtual ~Request();
            void parseHeaders();
            Logger* log();
            std::string& getUri();
            const char *getHTTPVersion();
            HTTP::VERB getVerb();
            json& getContext();
            void setArgs(PathMatcher::ArgResults* reqArgs);
            std::string getArg(const std::string& name) const;
            std::string getArg(int index) const;
            std::string getVerbAsString();
    };
}

#endif //HTTPSERV_REQUEST_H
