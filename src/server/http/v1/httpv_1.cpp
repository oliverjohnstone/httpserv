//
// Created by Oliver Johnstone on 17/04/2020.
//

#include <server/socket.h>
#include <server/error.h>
#include <boost/algorithm/string.hpp>
#include "httpv_1.h"

using namespace std;

HTTPServ::HTTPV1::HTTPV1(io::stream<InSocketStream> &inStream, io::stream<OutSocketStream> &outStream) :
    inStream(inStream), outStream(outStream) {
}

bool HTTPServ::HTTPV1::canUpgrade() {
    return false;
}

int HTTPServ::HTTPV1::readBody(char *buf, int numBytes) {
    return 0;
}

void HTTPServ::HTTPV1::parseHeaders() {
    while (headers.size() < 51) {
        std::string header;
        getLine(header);

        if (header.empty()) {
            break;
        }

        auto i = header.find(':');
        auto name = header.substr(0, i);
        auto value = header.substr(i + 1);

        boost::algorithm::trim(name);
        boost::algorithm::trim(value);

        headers[boost::algorithm::to_lower_copy(name)] = boost::algorithm::to_lower_copy(value);
    }

    if (headers.size() >= 50) {
        throw HTTPError::MaxHeaderSize();
    }
}

void HTTPServ::HTTPV1::parseRequestLine() {
    std::string request;
    getLine(request);

    if (request.empty()) {
        throw HTTPError::BadRequest();
    }

    std::vector<std::string> split, uriParts;
    boost::algorithm::split(split, request, boost::is_space());

    if (split.size() != 3) {
        throw HTTPError::BadRequest();
    }

    boost::algorithm::trim(split[0]); // Verb
    boost::algorithm::trim(split[2]); // Http Version
    boost::algorithm::to_upper(split[0]);

    boost::algorithm::split(uriParts, split[1], boost::is_any_of("?")); // Split Uri into path & query string

    boost::algorithm::trim(uriParts[0]); // Uri
    boost::algorithm::trim(uriParts[1]); // Query String

    verb = HTTP::VERB_MAP[split[0]];
    uri = HTTP::decode(uriParts[0]);
    httpVersion = split[2] == HTTP::VERSION_2 ? HTTP::VERSION_2 : HTTP::VERSION_1_1;

    parseQueryString(uriParts[1]);

    if (verb == HTTP::VERB::NONE) {
        throw HTTPError::NotImplemented();
    }
}

void HTTPServ::HTTPV1::getLine(std::string &out) {
    auto endOfLine = false;
    while (out.length() < HTTP::MAX_HEADER_SIZE && !inStream.eof()) {
        auto ch = inStream.get();
        if (inStream.fail()) {
            if (inStream.component()->didTimeout()) {
                throw SocketTimeout();
            }

            throw HTTPError::ServerError();
        }

        if (ch == '\r') {
            continue;
        } else if (ch == '\n') {
            endOfLine = true;
            break;
        }

        out.push_back(ch);
    }

    if (!endOfLine) {
        std::stringstream ss;
        ss << "Line exceeds maximum length of " << HTTP::MAX_HEADER_SIZE << ".";
        throw HTTPError::MaxHeaderSize(ss.str());
    }
}

void HTTPServ::HTTPV1::flush() {
    outStream.flush();
}

HTTPServ::HTTPV1::~HTTPV1() {
    // TODO
}

void HTTPServ::HTTPV1::writeStatusHeader(HTTPServ::HTTP::STATUS status) {
    outStream << HTTP::VERSION_1_1 << " " << status << " " << HTTP::STATUS_TEXT[status] << HTTP::PROTO_ENDL;
}

void HTTPServ::HTTPV1::writeHeaders(std::unordered_map<std::string, std::string> &headers) {
    for (auto &[name, value] : headers) {
        outStream << name << ": " << value << HTTP::PROTO_ENDL;
    }
}

void HTTPServ::HTTPV1::writeBody(const std::string &body) {
    outStream << HTTP::PROTO_ENDL << body;
}

void HTTPServ::HTTPV1::init() {
    parseRequestLine();
    parseHeaders();
}
