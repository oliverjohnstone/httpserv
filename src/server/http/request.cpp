//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/request.h>
#include <boost/algorithm/string.hpp>

std::map<std::string, HTTPServ::VERB> verbMap = {
    {"GET", HTTPServ::VERB::GET},
    {"POST", HTTPServ::VERB::POST},
    {"PUT", HTTPServ::VERB::PUT},
    {"DELETE", HTTPServ::VERB::DELETE},
    {"OPTIONS", HTTPServ::VERB::OPTIONS},
    {"HEAD", HTTPServ::VERB::HEAD},
};

HTTPServ::Request::Request(io::stream<InSocketStream> *stream) : stream(stream) {}

HTTPServ::Request::~Request() {
    delete stream;
}

void HTTPServ::Request::parseRequestLine() {
    std::string request;
    getLine(stream, request);

    if (request.empty()) {
        throw 400; // TODO - Place in enum http response headers BAD_REQUEST
    }

    std::vector<std::string> split;
    boost::algorithm::split(split, request, boost::is_space());

    if (split.size() != 3) {
        throw 400; // TODO - Place in enum http response headers BAD_REQUEST
    }

    boost::algorithm::trim(split[0]);
    boost::algorithm::trim(split[1]);
    boost::algorithm::trim(split[2]);
    boost::algorithm::to_upper(split[0]);

    verb = verbMap[split[0]];
    uri = split[1];
    httpVersion = split[2];

    if (verb == VERB::NONE) {
        throw 501; // TODO - Place in enum http response headers NOT_IMPLEMENTED
    }
}

void HTTPServ::Request::parseHeaders() {
    parseRequestLine();

    for (;;) {
        std::string header;
        getLine(stream, header);

        if (header.empty()) {
            break;
        }

        auto i = header.find(':');
        auto name = header.substr(0, i);
        auto value = header.substr(i + 1);

        boost::algorithm::trim(name);
        boost::algorithm::trim(value);

        headers[name] = value;
    }
}

void HTTPServ::Request::getLine(std::istream* is, std::string& out) {
    getline(*is, out);
    out.erase(out.find_last_not_of('\r') + 1);
}