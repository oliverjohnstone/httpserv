//
// Created by Oliver Johnstone on 23/03/2020.
//

#include <server/server.h>
#include <logger/logger.h>
#include <router/router.h>
#include <iostream>

using namespace HTTPServ;

class Test {
    public:
        void test(Request *req, Response *res) {
            res
                ->status(HTTP::STATUS::OK)
                ->header("Content-type", "application/json")
                ->end(R"({"hello": "world2"})");
        }
};

int main() {
    Logger logger(std::cout);
    Server server(8080, logger);
    Router router;

    server.attachRoutes(&router);

    const Test testClass;

    router.use("/test", [](Request *req, Response *res){
        res
            ->status(HTTP::STATUS::OK)
            ->header("Content-type", "application/json")
            ->end(R"({"hello": "world1"})");
    });

    router.use("/test2", Router::boundHandler(&Test::test, testClass));

    return server.run();
}