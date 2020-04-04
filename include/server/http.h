//
// Created by Oliver Johnstone on 01/04/2020.
//

#ifndef HTTPSERV_HTTP_H
#define HTTPSERV_HTTP_H

#include <unordered_map>
#include <string>

namespace HTTPServ::HTTP {

    extern const char * VERSION_1_1;
    extern const char * VERSION_2;
    extern const char * PROTO_ENDL;

    enum VERB {
        NONE,
        GET,
        POST,
        PUT,
        DELETE,
        OPTIONS,
        HEAD
    };

    enum STATUS {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NON_AUTHORITATIVE_INFO = 203,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,

        MULTIPLE_CHOICE = 300,
        MOVED_PERMANENTLY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        TEMPORARY_REDIRECT = 307,
        PERMANENT_REDIRECT = 308,

        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIMEOUT = 408,
        CONFLICT = 409,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PRECONDITION_FAILED = 412,
        PAYLOAD_TOO_LARGE = 413,
        URI_TOO_LONG = 414,
        UNSUPPORTED_MEDIA_TYPE = 415,
        RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED = 417,
        TEAPOT = 418,
        MISDIRECTED_REQUEST = 421,
        TOO_EARLY = 425,
        UPGRADE_REQUIRED = 426,
        PRECONDITION_REQUIRED = 428,
        TOO_MANY_REQUESTS = 429,
        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        UNAVAILABLE_FOR_LEGAL_REASONS = 451,

        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY = 502,
        SERVICE_UNAVAILABLE = 503,
        GATEWAY_TIMEOUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        VARIANT_ALSO_NEGOTIATES = 506,
        NOT_EXTENDED = 510,
        NETWORK_AUTHENTICATION_REQUIRED = 511
    };

    extern std::unordered_map<STATUS, const char *> STATUS_TEXT;
    extern std::unordered_map<std::string, HTTPServ::HTTP::VERB> VERB_MAP;
    extern std::unordered_map<HTTPServ::HTTP::VERB, const char *> VERB_TEXT;
}

#endif //HTTPSERV_HTTP_H
