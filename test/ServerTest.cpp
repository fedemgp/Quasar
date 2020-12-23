/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */
#include <gtest/gtest.h>
#include <cpprest/http_client.h>

#include "Server.h"

using namespace web;
using namespace http;
using namespace web::http::client;

TEST(Server, Responds200OnRightPost) {
    Server server;
    http_client client("http://localhost:8080");
    json::value postReq = json::value::parse("{\"satellites\":[{\"name\":\"kenobi\",\"distance\":100,\"message\":"
                                             "[\"este\",\"\",\"\",\"mensaje\",\"\"]},{\"name\":\"skywalker\",\"distance"
                                             "\":115.5,\"message\":[\"\",\"es\",\"\",\"\",\"secreto\"]},{\"name\":\""
                                             "sato\",\"distance\":142.7,\"message\":[\"este\",\"\",\"un\",\"\",\"\"]}]}");
    http_response res = client.request(methods::POST, "/topsecret", postReq).get();
    EXPECT_EQ(res.status_code(), status_codes::OK);
    res.extract_json().then([=] (json::value body) {
       json::object &position = body["position"].as_object();
       EXPECT_NEAR(position["x"].as_double(), -487.28, 0.01);
       EXPECT_NEAR(position["y"].as_double(), 1557.01, 0.01);
       EXPECT_EQ(body["message"].as_string(), "este es un mensaje secreto");
    });
}

TEST(Server, Responds404OnBadMessage) {
    Server server;
    http_client client("http://localhost:8080");
    json::value postReq = json::value::parse("{\"satellites\":[{\"name\":\"kenobi\",\"distance\":100,\"message\":"
                                             "[\"este\",\"\",\"\",\"mensaje\",\"\"]},{\"name\":\"skywalker\",\"distance"
                                             "\":115.5,\"message\":[\"\",\"es\",\"\",\"\",\"\"]},{\"name\":\""
                                             "sato\",\"distance\":14270,\"message\":[\"este\",\"\",\"un\",\"\",\"\"]}]}");
    http_response res = client.request(methods::POST, "/topsecret", postReq).get();
    EXPECT_EQ(res.status_code(), status_codes::NotFound);
}

TEST(Server, answerBadRequestIfThereIsAMissingKey) {
    Server server;
    http_client client("http://localhost:8080");
    json::value postReq = json::value::parse("{\"asdf\": 123}");
    http_response res = client.request(methods::POST, "/topsecret", postReq).get();
    EXPECT_EQ(res.status_code(), status_codes::BadRequest);
}