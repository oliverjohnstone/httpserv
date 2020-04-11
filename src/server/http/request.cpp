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
    this->logger->info("Request started");
}

HTTPServ::Request::~Request() {
    std::stringstream ss;
    ss << "Finished " << getVerbAsString() << " " << getUri();
    logger->info(ss.str().c_str());

    delete stream;
    delete logger; // New instance of logger as child
    delete args;
}

void HTTPServ::Request::parseRequestLine() {
    std::string request;
    getLine(stream, request);

    if (request.empty()) {
        throw HTTPError::BadRequest();
    }

    std::vector<std::string> split;
    boost::algorithm::split(split, request, boost::is_space());

    if (split.size() != 3) {
        throw HTTPError::BadRequest();
    }

    boost::algorithm::trim(split[0]);
    boost::algorithm::trim(split[1]);
    boost::algorithm::trim(split[2]);
    boost::algorithm::to_upper(split[0]);

    verb = HTTP::VERB_MAP[split[0]];
    uri = split[1];
    httpVersion = split[2] == HTTP::VERSION_2 ? HTTP::VERSION_2 : HTTP::VERSION_1_1;

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
    getline(*is, out);
    out.erase(out.find_last_not_of('\r') + 1);
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

std::string HTTPServ::Request::getArg(const std::string& name) const {
    auto str = args->at(name);

    if (str.empty()) {
        std::stringstream ss;
        ss << "There is no argument provided for: " << name << ".";
        throw std::invalid_argument(ss.str());
    }

    return str;
}

std::string HTTPServ::Request::getArg(int index) const {
    std::stringstream ss;
    ss << "captureGroup_" << index;
    return getArg(ss.str());
}
