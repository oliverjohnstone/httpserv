//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/request.h>
#include <boost/algorithm/string.hpp>
#include <server/error.h>

HTTPServ::Request::Request(io::stream<InSocketStream> *stream, Logger &logger) : stream(stream), args(nullptr) {
    boost::uuids::random_generator generateId;
    id = generateId();
    std::string uuidStr = boost::uuids::to_string(id);

    this->logger = logger.child(uuidStr);
}

HTTPServ::Request::~Request() {
    delete stream;
    delete logger;
    delete args;
}

void HTTPServ::Request::parseRequestLine() {
    std::string request;
    getLine(stream, request);

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
    auto endOfLine = false;
    while (out.length() < HTTP::MAX_HEADER_SIZE && !is->eof()) {
        auto ch = is->get();
        if (is->fail()) {
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

HTTPServ::Logger* HTTPServ::Request::log() {
    return logger;
}

std::string HTTPServ::Request::getVerbAsString() {
    auto str = HTTP::VERB_TEXT[verb];
    return str ?: "INVALID";
}

std::string& HTTPServ::Request::getUri() {
    return uri;
}

const char *HTTPServ::Request::getHTTPVersion() {
    return httpVersion;
}

HTTPServ::HTTP::VERB HTTPServ::Request::getVerb() {
    return verb;
}

json& HTTPServ::Request::getContext() {
    return context;
}

void HTTPServ::Request::setArgs(PathMatcher::ArgResults* reqArgs) {
    args = reqArgs;
}

std::string HTTPServ::Request::getArg(const std::string& name) {
    auto str = (*args)[name];

    if (str.empty()) {
        std::stringstream ss;
        ss << "There is no argument provided for: " << name << ".";
        throw std::invalid_argument(ss.str());
    }

    return str;
}

std::string HTTPServ::Request::getArg(int index) {
    std::stringstream ss;
    ss << "captureGroup_" << index;
    return getArg(ss.str());
}

std::string HTTPServ::Request::getQuery(const std::string& name) {
    return query[name];
}

void HTTPServ::Request::parseQueryString(const std::string &queryString) {
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
