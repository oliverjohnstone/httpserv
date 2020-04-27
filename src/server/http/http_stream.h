//
// Created by Oliver Johnstone on 17/04/2020.
//

#ifndef HTTPSERV_HTTP_STREAM_H
#define HTTPSERV_HTTP_STREAM_H

#include <server/stream.h>
#include <boost/iostreams/stream.hpp>
#include <server/request.h>
#include <server/response.h>
#include "v2/stream_manager.h"

namespace HTTPServ {
    class HTTPStream {
        private:
            StreamManager *v2StreamManager = nullptr;
            io::stream<InSocketStream> &inStream;
            io::stream<OutSocketStream> &outStream;
            const char * version = nullptr;
            int currentRequest = 0;
            int maxRequests;
            int connectionTimeout;

        public:
            HTTPStream(io::stream<InSocketStream> &inStream, io::stream<OutSocketStream> &outStream, int maxRequests, int connectionTimeout);
            virtual ~HTTPStream();
            HTTPServ::HTTPVersion* next();
    };
}

#endif //HTTPSERV_HTTP_STREAM_H
