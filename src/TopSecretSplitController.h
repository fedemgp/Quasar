/**
 * Created by Federico Manuel Gomez Peter 
 * on 23/12/20.
 */

#ifndef __TOPSECRETSPLITCONTROLLER_H__
#define __TOPSECRETSPLITCONTROLLER_H__

#include <cpprest/details/http_helpers.h>
#include <map>
#include <mutex>

#include "Decoder.h"
#include "HttpController.h"

class TopSecretSplitController: public HttpController {
private:
    Decoder &decoder;
    std::map<std::string, web::json::value> dataCollected;
    std::mutex mutex;
public:
    explicit TopSecretSplitController(Decoder &dec);
    ~TopSecretSplitController() override;
    /**
     * Devuelve el resultado de la decodificación si es que hay suficente información (los 3 satélites publicaron
     * información) o devuelve un mensaje de error.
     * @param message
     */
    void handleGet(web::http::http_request &message) override;
    void handlePut(web::http::http_request &message) override;
    /**
     * Cada método POST que se haga, insertará un elemento en el mapa dataCollected. Si llegan dos vices seguidas
     * una lectura del mismo satélite, el programa se quedará con la ultima lectura realizada.
     * @param message
     */
    void handlePost(web::http::http_request &message) override;
    void handleDelete(web::http::http_request &message) override;
    void handlePatch(web::http::http_request &messge) override;
    void handleHead(web::http::http_request &message) override;
    void handleOptions(web::http::http_request &message) override;
    void handleTrace(web::http::http_request &message) override;
    void handleConnect(web::http::http_request &message) override;
    void handleMerge(web::http::http_request &message) override;
};


#endif //__TOPSECRETSPLITCONTROLLER_H__
