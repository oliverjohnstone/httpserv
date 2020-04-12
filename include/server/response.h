//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_RESPONSE_H
#define HTTPSERV_RESPONSE_H

#include <boost/iostreams/stream.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include "stream.h"
#include "http.h"
#include "request.h"

namespace io = boost::iostreams;
using json = nlohmann::json;

namespace HTTPServ {
    class Response {
        private:
            io::stream<OutSocketStream> *stream;
            std::unordered_map<std::string, std::string> headers;
            bool headersSent = false;
            bool ended = false;
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
            HTTPServ::Response* end(const std::string& body = "");
            HTTPServ::Response* end(const char * body);
            HTTPServ::Response* end(const json& body);
            HTTPServ::Response* header(const std::string& name, const std::string& value);
            HTTP::STATUS getStatus();
    };
}

#endif //HTTPSERV_RESPONSE_H
