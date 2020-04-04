//
// Created by Oliver Johnstone on 29/03/2020.
//

#ifndef HTTPSERV_ERROR_H
#define HTTPSERV_ERROR_H

#include <mach/mach_types.h>
#include <string>
#include "http.h"

namespace HTTPServ {
    class HTTPError : public std::exception {
        private:
            HTTP::STATUS code;
            std::string message;

            HTTPError(HTTP::STATUS code, std::string& message) : code(code), message(message) {};

        public:

            [[nodiscard]] HTTP::STATUS getCode() const noexcept {
                return code;
            }

            [[nodiscard]] const std::string &getMessage() const noexcept {
                return message;
            }

            static HTTPError BadRequest(std::string message = "Bad Request") {
                return HTTPError(HTTP::STATUS::BAD_REQUEST, message);
            };

            static HTTPError ServerError(std::string message = "Server Error") {
                return HTTPError(HTTP::STATUS::INTERNAL_SERVER_ERROR, message);
            };

            static HTTPError NotImplemented(std::string message = "Not Implemented") {
                return HTTPError(HTTP::STATUS::NOT_IMPLEMENTED, message);
            };
    };
}

#endif //HTTPSERV_ERROR_H
