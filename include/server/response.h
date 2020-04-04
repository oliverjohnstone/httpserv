//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_RESPONSE_H
#define HTTPSERV_RESPONSE_H

#include <boost/iostreams/stream.hpp>
#include <map>
#include "stream.h"
#include "http.h"
#include "request.h"

namespace io = boost::iostreams;

namespace HTTPServ {
    class Response {
        private:
            io::stream<OutSocketStream> *stream;
            std::map<std::string, std::string> headers;
            bool headersSent = false;
            HTTP::STATUS statusCode = HTTP::STATUS::OK;
            const char * httpVersion;

            void sendHeaders();

        public:
            explicit Response(io::stream<OutSocketStream> *stream);
            virtual ~Response();
            void flush();
            void close();
            void syncWith(HTTPServ::Request *request);

            HTTPServ::Response* status(HTTPServ::HTTP::STATUS code);
            HTTPServ::Response* end(const std::string &body = "");
            HTTPServ::Response* header(const std::string& name, std::string value);
    };
}

#endif //HTTPSERV_RESPONSE_H
