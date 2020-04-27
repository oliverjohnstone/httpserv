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
#include <vector>

namespace io = boost::iostreams;

namespace HTTPServ {
    enum ALPN {
        HTTP1_1,
        H2
    };

    const std::tuple<const char *, ALPN> ALPN_MAP[] = {
        {"h2", ALPN::H2},
        {"http/1.1", ALPN::HTTP1_1}
    };

    class InSocketStream {
        private:
            int socketFd;
            SSL *ssl = nullptr;
            bool timedOut = false;
            ALPN alpn;

        public:
            typedef char char_type;
            struct category : io::source_tag, io::closable_tag {};
            explicit InSocketStream(int socketFd);
            explicit InSocketStream(int socketFd, SSL *ssl, ALPN alpn);
            std::streamsize read(char * buf, std::streamsize chars);
            void close();
            bool didTimeout();
            bool isH2Socket();
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
