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

namespace io = boost::iostreams;

namespace HTTPServ {
    enum VERB {NONE, GET, POST, PUT, DELETE, OPTIONS, HEAD};

    class Request {
        private:
            io::stream<InSocketStream> *stream;
            std::map<std::string, std::string> headers;
            VERB verb = NONE;
            std::string uri;
            std::string httpVersion;
            Logger* logger;
            boost::uuids::uuid id;

            void parseRequestLine();
            static void getLine(std::istream* is, std::string& out);

        public:
            Request(io::stream<InSocketStream>* stream, Logger& logger);
            virtual ~Request();
            void parseHeaders();
            Logger* log();
    };
}

#endif //HTTPSERV_REQUEST_H
