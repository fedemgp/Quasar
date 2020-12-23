/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <cpprest/http_listener.h>
#include <cpprest/details/http_helpers.h>
#include <functional>

#include "Decoder.h"
#include "HttpController.h"

class Server {
private:
    web::http::experimental::listener::http_listener listener;
    std::map<std::string, HttpController *> controllers;
    Decoder decoder;
public:
    Server();
    ~Server();
    /**
     * Callback que se ejecuta cuando se realiza un POST http. Se parceará la uri para ejecutar
     * el controlador determinado acorde al endpoint.
     * @param message
     */
    void handlePost(web::http::http_request message);
    /**
     * Callback que se ejecuta cuando se realiza un GET http. Se parceará la uri para ejecutar
     * el controlador determinado acorde al endpoint.
     * @param message
     */
    void handleGet(web::http::http_request message);
    /**
     * Retorna el endpoint al que se registró el servidor REST.
     * @return endpoint como string
     */
    std::string getEndpoint();
    /**
     * Retorna el path solicitado como un vector de strings
     * @param http_request
     * @return
     */
    static std::vector<utility::string_t> requestPath(const web::http::http_request &httpRequest);
};


#endif //__SERVER_H__
