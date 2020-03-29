//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_REQUEST_H
#define HTTPSERV_REQUEST_H

#include <boost/iostreams/stream.hpp>
#include <map>
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

            void parseRequestLine();
            static void getLine(std::istream* is, std::string& out);

        public:
            explicit Request(io::stream<InSocketStream> *stream);
            virtual ~Request();
            void parseHeaders();
    };
}

#endif //HTTPSERV_REQUEST_H
