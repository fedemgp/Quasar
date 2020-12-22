/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <cpprest/http_listener.h>
#include <cpprest/details/http_helpers.h>

#include "Decoder.h"

class Server {
private:
    web::http::experimental::listener::http_listener listener;
    Decoder decoder;

public:
    Server();
    ~Server();
    /**
     * Callback que se ejecuta cuando se realiza un POST http. Se parceará el mensaje para
     * ejecutar una lambda acorde al endpoint solicitado
     * @param message
     */
    void handlePost(web::http::http_request message);
    /**
     * Retorna el endpoint al que se registró el servidor REST.
     * @return endpoint como string
     */
    std::string getEndpoint();
};


#endif //__SERVER_H__
