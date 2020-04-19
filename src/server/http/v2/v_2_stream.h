//
// Created by Oliver Johnstone on 19/04/2020.
//

#ifndef HTTPSERV_V_2_STREAM_H
#define HTTPSERV_V_2_STREAM_H

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>

namespace HTTPServ {
    class V2InputStream {
        private:
            int socketFd;

        public:
            typedef char char_type;
            struct category : boost::iostreams::source_tag, boost::iostreams::closable_tag {};

            explicit V2InputStream(int inFd);
            std::streamsize read(char * buf, std::streamsize chars);
            void close();
    };

    class V2OutputStream {
        private:
            int socketFd;

        public:
            typedef char char_type;
            struct category : boost::iostreams::sink_tag, boost::iostreams::closable_tag {};

            explicit V2OutputStream(int outFd);
            std::streamsize write(const char* s, std::streamsize n);
            void close();
    };
}

#endif //HTTPSERV_V_2_STREAM_H
