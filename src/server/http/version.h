//
// Created by Oliver Johnstone on 17/04/2020.
//

#ifndef HTTPSERV_VERSION_H
#define HTTPSERV_VERSION_H

#include <server/http.h>
#include <map>
#include <string>

namespace HTTPServ {
    class HTTPVersion {
        private:
            std::unordered_map<std::string, std::string> query;

        protected:
            HTTP::VERB verb = HTTP::VERB::NONE;
            std::string uri;
            std::unordered_map<std::string, std::string> headers;
            const char* httpVersion;

            void parseQueryString(const std::string &queryString);

        public:
            virtual ~HTTPVersion() = default;

            HTTP::VERB getVerb();
            std::string& getUri();
            std::string& getQuery(const std::string& name);
            std::string& getHeader(const std::string& name);
            const char *getVersion();

            virtual int readBody(char *buf, int numBytes) = 0;
            virtual void init() = 0;
            virtual void flush() = 0;
            virtual void writeStatusHeader(HTTP::STATUS status) = 0;
            virtual void writeHeaders(std::unordered_map<std::string, std::string> &headers) = 0;
            virtual void writeBody(const std::string &body) = 0;
    };
}

#endif //HTTPSERV_VERSION_H
