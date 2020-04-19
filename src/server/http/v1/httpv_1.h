//
// Created by Oliver Johnstone on 17/04/2020.
//

#ifndef HTTPSERV_HTTPV_1_H
#define HTTPSERV_HTTPV_1_H

#include <server/stream.h>
#include <boost/iostreams/stream.hpp>
#include "../version.h"
#include "../http_stream.h"

namespace HTTPServ {
    class HTTPV1 : public HTTPVersion {
        private:
            io::stream<InSocketStream> &inStream;
            io::stream<OutSocketStream> &outStream;

            void parseHeaders();
            void parseRequestLine();
            void getLine(std::string &out);

        public:
            HTTPV1(io::stream<InSocketStream> &inStream, io::stream<OutSocketStream> &outStream);
            bool canUpgrade();
            int readBody(char *buf, int numBytes) override;
            void flush() override;
            ~HTTPV1();
            void init() override;
            void writeStatusHeader(HTTP::STATUS status) override;
            void writeHeaders(std::unordered_map<std::string, std::string> &headers) override;
            void writeBody(const std::string &body) override;
    };
}

#endif //HTTPSERV_HTTPV_1_H
