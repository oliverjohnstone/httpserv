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
#include "stream.h"
#include "http.h"

namespace io = boost::iostreams;

namespace HTTPServ {
    class Request {
        private:
            io::stream<InSocketStream> *stream;
            std::map<std::string, std::string> headers;
            HTTP::VERB verb = HTTP::VERB::NONE;
            std::string uri;
            std::string httpVersion;
            Logger* logger;
            boost::uuids::uuid id;

            void parseRequestLine();
            static void getLine(std::istream* is, std::string& out);
            std::string getVerbAsString();

        public:
            Request(io::stream<InSocketStream>* stream, Logger& logger);
            virtual ~Request();
            void parseHeaders();
            Logger* log();
            const std::string& getUri() const;
    };
}

#endif //HTTPSERV_REQUEST_H
