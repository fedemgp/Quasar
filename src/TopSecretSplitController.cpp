/**
 * Created by Federico Manuel Gomez Peter 
 * on 23/12/20.
 */

#include "TopSecretSplitController.h"
#include "Server.h"

using namespace web;
using namespace http;

TopSecretSplitController::TopSecretSplitController(Decoder &dec): decoder(dec) {}

TopSecretSplitController::~TopSecretSplitController() {}

void TopSecretSplitController::handlePost(web::http::http_request &message) {
    std::vector<utility::string_t> path = Server::requestPath(message);
    utility::string_t  &satelite = path.back();
    if (satelite != KENOBI_NAME && satelite != SKYWALKER_NAME && satelite != SATO_NAME) {
        message.reply(status_codes::BadRequest, "Wrong satelite name");
        return;
    }
    json::value value = message.extract_json().get();
    value["name"] = json::value::string(satelite);
    std::lock_guard<std::mutex> lock(this->mutex);
    this->dataCollected[satelite] = value;
    message.reply(status_codes::OK);
}

void TopSecretSplitController::handleGet(http_request &message) {
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->dataCollected.size() != 3) {
        message.reply(status_codes::NotFound, "Not enough data to calculate information");
        return;
    }
    std::vector<json::array> messageToDecode;
    std::map<std::string, float> distances;
    for (auto &node: dataCollected) {
        json::object &sat = node.second.as_object();
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
    // borro el mapa para reiniciar el proceso
    this->dataCollected.clear();
    message.reply(status_codes::OK, response);

}

void TopSecretSplitController::handlePut(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::PUT);
}

void TopSecretSplitController::handleDelete(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::DEL);
}

void TopSecretSplitController::handlePatch(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::PATCH);
}

void TopSecretSplitController::handleHead(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::HEAD);
}

void TopSecretSplitController::handleOptions(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::OPTIONS);
}

void TopSecretSplitController::handleTrace(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::TRCE);
}

void TopSecretSplitController::handleConnect(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::CONNECT);
}

void TopSecretSplitController::handleMerge(http_request &message) {
    message.reply(status_codes::NotImplemented, methods::MERGE);
}

