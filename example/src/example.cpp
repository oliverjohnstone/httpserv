//
// Created by Oliver Johnstone on 23/03/2020.
//

#include <server/server.h>
#include <logger/logger.h>
#include <router/router.h>

using namespace HTTPServ;
using namespace nlohmann;

int main() {
    Logger logger(std::cout);
    Server server(8080, logger);
    Router router;

    server.attachRouter(&router);

    router.get("/test/:arg1/:arg2", [](Request *req, Response *res) {
        res->end({
            {
                "args", {
                    req->getArg("arg1"),
                    req->getArg("arg2")
                }
            },
            {
                "query", {
                    req->getQuery("testQuery"),
                    req->getQuery("hello")
                }
            }
        });
    });

    return server.run();
}