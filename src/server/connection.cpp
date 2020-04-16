//
// Created by Oliver Johnstone on 28/03/2020.
//

#include <server/connection.h>
#include <server/error.h>
#include <sstream>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <server/socket.h>

HTTPServ::Connection::Connection(io::stream<InSocketStream> &in, io::stream<OutSocketStream> &out,
    int maxRequests, int socketTimeout) : in(in), out(out), maxRequests(maxRequests), socketTimeout(socketTimeout) {
}

void HTTPServ::Connection::handleRequests(const std::vector<std::reference_wrapper<HTTPServ::Router>> &routers, Logger &parentLogger) {
    for (auto i = 0; i < maxRequests; i++) {
        boost::uuids::random_generator generateId;
        auto id = generateId();
        std::string uuidStr = boost::uuids::to_string(id);
        auto logger = parentLogger.child(uuidStr);
        Request req(in, logger);
        Response res(out, maxRequests, socketTimeout);

        try {
            auto found = false;

            req.parseHeaders();
            res.syncWith(req);

            logRequestStart(logger);

            for (auto &routerRef : routers) {
                auto handler = routerRef.get().getHandler(req.getVerb(), req.getUri());
                found |= handler(req, res);
            }

            if (!found) {
                res
                    .status(HTTP::STATUS::NOT_FOUND)
                    .header("Content-Type", "text/plain")
                    .end("Not Found");
            }

            logRequestEnd(logger, req, res);

            if (req.shouldClose()) {
                break;
            }
        } catch (SocketTimeout &e) {
            break;
        } catch (HTTPError& e) {
            logger.warn(e.getMessage().c_str());
            res
                .status(e.getCode())
                .header("Content-Type", "text/plain")
                .end(e.getMessage());
        } catch (std::exception& e) {
            std::stringstream ss;
            ss << "Exception of type (" << e.what() << ") thrown whilst handling request.";
            logger.error(ss.str().c_str());
            res
                .status(HTTP::STATUS::INTERNAL_SERVER_ERROR)
                .header("Content-Type", "text/plain")
                .end("Internal Server Error");
        } catch (...) {
            logger.error("Exception of unknown type thrown whilst handling request");
            res
                .status(HTTP::STATUS::INTERNAL_SERVER_ERROR)
                .header("Content-Type", "text/plain")
                .end("Internal Server Error");
        }
    }
}

void HTTPServ::Connection::logRequestStart(Logger &logger) {
    logger.info("Request started");
}

void HTTPServ::Connection::logRequestEnd(Logger &logger, Request &req, Response &res) {
    std::stringstream ss;
    ss << "[" << req.getVerbAsString() << " " << res.getStatus() << "] " << req.getUri();
    logger.info(ss.str().c_str());
}
