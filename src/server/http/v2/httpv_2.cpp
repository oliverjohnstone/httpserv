//
// Created by Oliver Johnstone on 17/04/2020.
//

#include "httpv_2.h"

HTTPServ::HTTPV2::HTTPV2(HTTPV1 *v1) : v1(v1) {
    // TODO
    throw std::runtime_error("not implemented");
}

HTTPServ::HTTPV2::~HTTPV2() {
    delete v1;
}

int HTTPServ::HTTPV2::readBody(char *buf, int numBytes) {
    // TODO
    return 0;
}

HTTPServ::HTTPV2* HTTPServ::HTTPV2::upgrade() {
    // TODO
    return this;
}

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

void HTTPServ::HTTPV2::init() {
    // TODO
}
