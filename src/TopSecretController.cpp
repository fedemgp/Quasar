/**
 * Created by Federico Manuel Gomez Peter 
 * on 23/12/20.
 */

#include "TopSecretController.h"

using namespace web;
using namespace http;

TopSecretController::TopSecretController(Decoder &dec): decoder(dec) {}

TopSecretController::~TopSecretController() {}

void TopSecretController::handlePost(http_request &message) {
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
            message.reply(status_codes::BadRequest, "Unknown error in onTopSecretPost");
        }
    });
}

void TopSecretController::handleGet(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::GET);
}

void TopSecretController::handlePut(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::PUT);
}

void TopSecretController::handleDelete(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::DEL);
}

void TopSecretController::handlePatch(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::PATCH);
}

void TopSecretController::handleHead(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::HEAD);
}

void TopSecretController::handleOptions(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::OPTIONS);
}

void TopSecretController::handleTrace(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::TRCE);
}

void TopSecretController::handleConnect(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::CONNECT);
}

void TopSecretController::handleMerge(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::MERGE);
}


