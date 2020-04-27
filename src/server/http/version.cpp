//
// Created by Oliver Johnstone on 17/04/2020.
//

#include <boost/algorithm/string.hpp>
#include "version.h"

using namespace std;

void HTTPServ::HTTPVersion::parseQueryString(const std::string &queryString) {
    std::vector<std::string> splitOnAmp, splitOnEq;
    boost::algorithm::split(splitOnAmp, queryString, boost::is_any_of("&"));

    for (const auto &pair : splitOnAmp) {
        splitOnEq.clear();
        boost::algorithm::split(splitOnEq, pair, boost::is_any_of("="));

        auto name = HTTP::decode(boost::algorithm::trim_copy(splitOnEq[0]));
        auto value = HTTP::decode(boost::algorithm::trim_copy(splitOnEq[1]));

        query[name] = value;
    }
}

std::string& HTTPServ::HTTPVersion::getQuery(const std::string& name) {
    return query[name];
}

HTTPServ::HTTP::VERB HTTPServ::HTTPVersion::getVerb() {
    return verb;
}

std::string& HTTPServ::HTTPVersion::getUri() {
    return uri;
}

std::string& HTTPServ::HTTPVersion::getHeader(const std::string& name) {
    return headers[name];
}

