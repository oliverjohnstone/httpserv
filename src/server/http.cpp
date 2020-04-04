//
// Created by Oliver Johnstone on 01/04/2020.
//

#include <server/http.h>

namespace HTTPServ::HTTP {
    const char * VERSION_1_1 = "HTTP/1.1";
    const char * VERSION_2 = "HTTP/2.0";

    std::unordered_map<STATUS, const char *> STATUS_TEXT = {
        {STATUS::OK, "OK"},
        {STATUS::CREATED, "Created"},
        {STATUS::ACCEPTED, "Accepted"},
        {STATUS::NON_AUTHORITATIVE_INFO, "Non-Authoritative Information"},
        {STATUS::NO_CONTENT, "No Content"},
        {STATUS::RESET_CONTENT, "Reset Content"},
        {STATUS::PARTIAL_CONTENT, "Partial Content"},

        {STATUS::MULTIPLE_CHOICE, "Multiple Choice"},
        {STATUS::MOVED_PERMANENTLY, "Moved Permanently"},
        {STATUS::FOUND, "Found"},
        {STATUS::SEE_OTHER, "See Other"},
        {STATUS::NOT_MODIFIED, "Not Modified"},
        {STATUS::TEMPORARY_REDIRECT, "Temporary Redirect"},
        {STATUS::PERMANENT_REDIRECT, "Permanent Redirect"},

        {STATUS::BAD_REQUEST, "Bad Request"},
        {STATUS::UNAUTHORIZED, "Unauthorized"},
        {STATUS::PAYMENT_REQUIRED, "Payment Required"},
        {STATUS::FORBIDDEN, "Forbidden"},
        {STATUS::NOT_FOUND, "Not Found"},
        {STATUS::METHOD_NOT_ALLOWED, "Method Not Allowed"},
        {STATUS::NOT_ACCEPTABLE, "Not Acceptable"},
        {STATUS::PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
        {STATUS::REQUEST_TIMEOUT, "Request Timeout"},
        {STATUS::CONFLICT, "Conflict"},
        {STATUS::GONE, "Gone"},
        {STATUS::LENGTH_REQUIRED, "Length Required"},
        {STATUS::PRECONDITION_FAILED, "Precondition Failed"},
        {STATUS::PAYLOAD_TOO_LARGE, "Payload Too Large"},
        {STATUS::URI_TOO_LONG, "URI Too Long"},
        {STATUS::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
        {STATUS::RANGE_NOT_SATISFIABLE, "Range Not Satisfiable"},
        {STATUS::EXPECTATION_FAILED, "Expectation Failed"},
        {STATUS::TEAPOT, "I'm a teapot"},
        {STATUS::MISDIRECTED_REQUEST, "Misdirected Request"},
        {STATUS::TOO_EARLY, "Too Early"},
        {STATUS::UPGRADE_REQUIRED, "Upgrade Required"},
        {STATUS::PRECONDITION_REQUIRED, "Precondition Required"},
        {STATUS::TOO_MANY_REQUESTS, "Too Many Requests"},
        {STATUS::REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"},
        {STATUS::UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons"},

        {STATUS::INTERNAL_SERVER_ERROR, "Internal Server Error"},
        {STATUS::NOT_IMPLEMENTED, "Not Implemented"},
        {STATUS::BAD_GATEWAY, "Bad Gateway"},
        {STATUS::SERVICE_UNAVAILABLE, "Service Unavailable"},
        {STATUS::GATEWAY_TIMEOUT, "Gateway Timeout"},
        {STATUS::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"},
        {STATUS::VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"},
        {STATUS::NOT_EXTENDED, "Not Extended"},
        {STATUS::NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"}
    };
}