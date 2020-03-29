//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_RESPONSE_H
#define HTTPSERV_RESPONSE_H

#include <boost/iostreams/stream.hpp>
#include "stream.h"

namespace io = boost::iostreams;

namespace HTTPServ {
    class Response {
        private:
            io::stream<OutSocketStream> *stream;

        public:
            explicit Response(io::stream<OutSocketStream> *stream);
            virtual ~Response();
            void flush();
            void close();

            HTTPServ::Response* status(int code);
            HTTPServ::Response* end(const std::string &body);
    };
}

#endif //HTTPSERV_RESPONSE_H
