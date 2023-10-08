#include "JwtFilter.h"

using namespace api::v1::filters;

void JwtFilter::doFilter(const HttpRequestPtr &request, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    if (request->getMethod() == HttpMethod::Options) return fccb();

    const std::string& token = request->getHeader("Authorization");

    if (token.length() < 7) {
        Json::Value resultJson;
        resultJson["error"] = "No header authentication!";
        resultJson["status"] = 0;

        auto res = HttpResponse::newHttpJsonResponse(resultJson);
        res->setStatusCode(k401Unauthorized);

        return fcb(res);
    }

    std::map<std::string, any> jwtAttributes = JWT::decodeToken(token.substr(7));
    if (jwtAttributes.empty()) {
        Json::Value resultJson;
        resultJson["error"] = "Token is invalid!";
        resultJson["status"] = 0;

        auto res = HttpResponse::newHttpJsonResponse(resultJson);
        res->setStatusCode(k401Unauthorized);

        return fcb(res);
    }

    for (auto& attribute : jwtAttributes)
        request->getAttributes()->insert("jwt_" + attribute.first, attribute.second);

    return fccb();
}
