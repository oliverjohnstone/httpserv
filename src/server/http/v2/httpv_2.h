//
// Created by Oliver Johnstone on 17/04/2020.
//

#ifndef HTTPSERV_HTTPV_2_H
#define HTTPSERV_HTTPV_2_H

#include "../version.h"

namespace HTTPServ {
    class HTTPV2 : public HTTPVersion {
        public:
            explicit HTTPV2(); // TODO - Constructor from new filtered streams

            int readBody(char *buf, int numBytes) override;
            void flush() override;
            void writeStatusHeader(HTTP::STATUS status) override;
            void writeHeaders(std::unordered_map<std::string, std::string> &headers) override;
            void writeBody(const std::string &body) override;
    };
}

#endif //HTTPSERV_HTTPV_2_H
