/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */

#include <map>
#include <cmath>

#include "Server.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::details;

Server::Server(): listener("http://127.0.0.1:8080/topsecret") {
    decoder.addSatelite(KENOBI_NAME, {-500, -200});
    decoder.addSatelite(SKYWALKER_NAME, {100, -100});
    decoder.addSatelite(SATO_NAME, {500, 100});
    listener.support(methods::POST, std::bind(&Server::handlePost, this, std::placeholders::_1));
    listener.open().wait();
}

Server::~Server() {
    listener.close().wait();
}

void Server::handlePost(http_request message) {
    message.extract_json().then([=](json::value request) {
        try {
            json::array &satellites = request.at("satellites").as_array();
            std::vector<json::array> messageToDecode;
            std::map<std::string, float> distances;
            for (auto &satelite: satellites) {
                json::object &sat = satelite.as_object();
                distances.emplace(sat.at("name").as_string(), sat.at("distance").as_double());
                messageToDecode.emplace_back(sat.at("message").as_array());
            }
            Position pos = decoder.getLocation(distances);
            std::string decodedMsg = decoder.getString(messageToDecode);

            json::value response;
            json::value posResult;
            posResult["x"] = json::value::number(pos.x);
            posResult["y"] = json::value::number(pos.y);
            response["message"] = json::value::string(decodedMsg);
            response["position"] = posResult;
            message.reply(status_codes::OK, response);
        } catch(json::json_exception &e) {
            message.reply(status_codes::BadRequest, e.what());
        } catch (std::exception &e) {
            message.reply(status_codes::NotFound);
        } catch (...) {
            message.reply(status_codes::NotFound, "Unknown error in handle post");
        }
    });
}

std::string Server::getEndpoint() {
    return this->listener.uri().to_string();
}
