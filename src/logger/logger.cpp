//
// Created by Oliver Johnstone on 23/03/2020.
//

#include "logger/logger.h"

using namespace std;

auto RED = "\033[1;31m";
auto YELLOW = "\033[1;33m";

mutex logMutex;

void HTTPServ::Logger::log(const char *msg, LOG_LEVEL level) {
    char buffer[100];
    auto tm = time(nullptr);
    strftime(buffer, 100, "[%c] ", gmtime(&tm));

    lock_guard<mutex> lock(logMutex);

    logStream << buffer;
    logLevelText(buffer, level);
    logStream << buffer;

    if (!id.empty()) {
        logStream << "(" << id << ") ";
    }

    logStream << msg << "\n";
}

void HTTPServ::Logger::warn(const char *msg) {
    log(msg, LOG_LEVEL::WARN);
}

void HTTPServ::Logger::info(const char *msg) {
    log(msg, LOG_LEVEL::INFO);
}

void HTTPServ::Logger::error(const char *msg) {
    log(msg, LOG_LEVEL::ERROR);
}

HTTPServ::Logger::Logger(ostream& logStream, bool enableColour) : logStream(logStream), enableColour(enableColour) {}

HTTPServ::Logger::Logger(std::ostream& stream, bool enableColour, std::string& id) : id(id), logStream(stream), enableColour(enableColour) {}

void HTTPServ::Logger::logLevelText(char* out, LOG_LEVEL level) {
    auto format = "%s%s%s: ";
    auto end = "\033[0m";

    switch (level) {
        case LOG_LEVEL::WARN:
            sprintf(out, format, enableColour ? YELLOW : "", "WARN", enableColour ? end : "");
            break;
        case LOG_LEVEL::ERROR:
            sprintf(out, format, enableColour ? RED : "", "ERROR", enableColour ? end : "");
            break;
        default:
            sprintf(out, format, "", "INFO", "");
    }
}

HTTPServ::Logger* HTTPServ::Logger::child(std::string& childId) {
    return new Logger(this->logStream, this->enableColour, childId);
}