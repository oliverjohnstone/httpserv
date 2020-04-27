//
// Created by Oliver Johnstone on 17/04/2020.
//

#include "httpv_2.h"

using namespace std;

HTTPServ::HTTPV2::HTTPV2() {
    // TODO
    throw std::runtime_error("not implemented");
}

int HTTPServ::HTTPV2::readBody(char *buf, int numBytes) {
    // TODO
    return 0;
}

//HTTPServ::HTTPV2* HTTPServ::HTTPV2::upgrade() {
//    v1->writeStatusHeader((HTTP::STATUS)101); // TODO - Should we include this in the enum (Server impl specific)?
//    unordered_map<string, string> headers = {
//        {"connection", "upgrade"},
//        {"upgrade", "h2c"} // TODO - I think this should be determined by if the connection is over tls
//    };
//    v1->writeHeaders(headers);
//    v1->flush();
//    return this;
//}

void HTTPServ::HTTPV2::flush() {
    // TODO
}

void HTTPServ::HTTPV2::writeStatusHeader(HTTPServ::HTTP::STATUS status) {
    // TODO
}

void HTTPServ::HTTPV2::writeHeaders(std::unordered_map<std::string, std::string> &headers) {
    // TODO
}

void HTTPServ::HTTPV2::writeBody(const std::string &body) {
    // TODO
}
