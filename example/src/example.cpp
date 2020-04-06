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
            res->header("test-header", "hello");
            std::cout << req->getContext()["test"]["test"].get<std::string>() << std::endl;
        }
};

int main() {
    Logger logger(std::cout);
    Server server(8080, logger);
    Router router;

    server.attachRouter(&router);

    const Test testClass;

    router.use("/test",
        [](Request *req, Response *res){
            req->getContext()["test"]["test"] = "Hi There";

            res
                ->status(HTTP::STATUS::OK)
                ->header("Content-type", "application/json");
        },
        Router::boundHandler(&Test::test, testClass)
    );

    router.get("/test", [](Request *req, Response *res){
        std::cout << "GET /test" << std::endl;
        res->end(R"({"hello": "GET"})");
    });

    router.post("/test", [](Request *req, Response *res){
        std::cout << "POST /test" << std::endl;
        res->end(R"({"hello": "POST"})");
    });

    return server.run();
}