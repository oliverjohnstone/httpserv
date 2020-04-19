//
// Created by Oliver Johnstone on 17/04/2020.
//

#ifndef HTTPSERV_HTTPV_2_H
#define HTTPSERV_HTTPV_2_H

#include "../version.h"
#include "../v1/httpv_1.h"

namespace HTTPServ {
    class HTTPV2 : public HTTPVersion {
        private:
            HTTPV1 *v1 = nullptr;

        public:
            explicit HTTPV2(HTTPV1 *v1);
            // TODO - Constructor from new filtered streams
            ~HTTPV2();

            int readBody(char *buf, int numBytes) override;
            HTTPV2* upgrade();
            void flush() override;
            void writeStatusHeader(HTTP::STATUS status) override;
            void writeHeaders(std::unordered_map<std::string, std::string> &headers) override;
            void writeBody(const std::string &body) override;
            void init() override;
    };
}

#endif //HTTPSERV_HTTPV_2_H
