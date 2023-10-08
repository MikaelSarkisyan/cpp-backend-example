#include <drogon/drogon.h>

using namespace drogon;

int main() {
    app().loadConfigFile("./config.json");

    app().registerPostHandlingAdvice([](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });

    app().run();

    return 0;
}
