//
// Created by Oliver Johnstone on 13/04/2020.
//

#ifndef HTTPSERV_TLS_H
#define HTTPSERV_TLS_H

#include <string>

namespace HTTPServ::TLS {
    typedef struct {
        std::string certFile;
        std::string keyFile;
    } Config;
}

#endif //HTTPSERV_TLS_H
