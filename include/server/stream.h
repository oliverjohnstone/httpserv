//
// Created by Oliver Johnstone on 28/03/2020.
//

#ifndef HTTPSERV_STREAM_H
#define HTTPSERV_STREAM_H

#include <iosfwd>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <ios>
#include <openssl/ssl.h>

namespace io = boost::iostreams;

namespace HTTPServ {
    class InSocketStream {
        private:
            int socketFd;
            SSL *ssl = nullptr;

        public:
            typedef char char_type;
            struct category : io::source_tag, io::closable_tag {};

            explicit InSocketStream(int socketFd);
            explicit InSocketStream(int socketFd, SSL *ssl);
            std::streamsize read(char * buf, std::streamsize chars);
            void close();
    };

    class OutSocketStream {
        private:
            int socketFd;
            SSL *ssl = nullptr;

        public:
            typedef char char_type;
            struct category : io::sink_tag, io::closable_tag {};

            explicit OutSocketStream(int socketFd);
            explicit OutSocketStream(int socketFd, SSL *ssl);
            std::streamsize write(const char* s, std::streamsize n);
            void close();
    };
}


#endif //HTTPSERV_STREAM_H
