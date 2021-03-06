//
// Created by Oliver Johnstone on 23/03/2020.
//

#ifndef HTTPSERV_LOGGER_H
#define HTTPSERV_LOGGER_H

#include <iostream>

namespace HTTPServ {

    class Logger {
        private:
            enum LOG_LEVEL {
                WARN = 1,
                INFO = 2,
                ERROR = 3
            };

            std::ostream& logStream;
            bool enableColour;

            // child properties
            std::string id;

            void log(const char* msg, LOG_LEVEL level);
            void logLevelText(char* out, LOG_LEVEL level);

        public:
            explicit Logger(std::ostream& stream, bool enableColour = true);
            Logger(std::ostream& stream, bool enableColour, std::string& id);

            void warn(const char* msg);
            void info(const char* msg);
            void error(const char* msg);
            Logger child(std::string& id);
    };
}

#endif //HTTPSERV_LOGGER_H
