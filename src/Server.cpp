/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */

#include <map>
#include <cmath>

#include "Server.h"
#include "TopSecretController.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::details;

Server::Server(): listener("http://127.0.0.1:8080") {
    decoder.addSatelite(KENOBI_NAME, {-500, -200});
    decoder.addSatelite(SKYWALKER_NAME, {100, -100});
    decoder.addSatelite(SATO_NAME, {500, 100});

    controllers.emplace("topsecret", new TopSecretController(decoder));
//    controllers.emplace("topsecret_split", [&](http_request request){this->onTopSecretSplitPost(request);});

    listener.support(methods::POST, std::bind(&Server::handlePost, this, std::placeholders::_1));
    listener.open().wait();
}

Server::~Server() {
    listener.close().wait();
    for (auto &node: controllers) {
        delete node.second;
    }
}

void Server::handlePost(http_request message) {
    try {
        std::vector<utility::string_t> paths = requestPath(message);
        if (paths.size() == 0) {
            message.reply(status_codes::NotFound);
            return;
        }

        if (controllers.find(*paths.begin()) == controllers.end()) {
            message.reply(status_codes::NotFound);
            return;
        }

        HttpController *controller = controllers.at(*paths.begin());
        controller->handlePost(message);
    } catch (std::exception &e) {
        message.reply(status_codes::BadRequest, e.what());
    } catch (...) {
        message.reply(status_codes::BadRequest, "Unknown error in handlePost");
    }
}

std::string Server::getEndpoint() {
    return this->listener.uri().to_string();
}

std::vector<utility::string_t> Server::requestPath(const http_request &httpRequest) {
    auto relativePath = uri::decode(httpRequest.relative_uri().path());
    return uri::split_path(relativePath);
}
