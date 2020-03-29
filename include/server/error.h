//
// Created by Oliver Johnstone on 29/03/2020.
//

#ifndef HTTPSERV_ERROR_H
#define HTTPSERV_ERROR_H

#include <mach/mach_types.h>
#include <string>

namespace HTTPServ {
    class HTTPError : public std::exception {
        private:
            int code;
            std::string message;

            HTTPError(int code, std::string& message) : code(code), message(message) {};

        public:

            [[nodiscard]] int getCode() const noexcept {
                return code;
            }

            [[nodiscard]] const std::string &getMessage() const noexcept {
                return message;
            }

            static HTTPError BadRequest(std::string message = "Bad Request") {
                return HTTPError(400, message);
            };

            static HTTPError ServerError(std::string message = "Server Error") {
                return HTTPError(500, message);
            };

            static HTTPError NotImplemented(std::string message = "Not Implemented") {
                return HTTPError(501, message);
            };
    };
}

#endif //HTTPSERV_ERROR_H
