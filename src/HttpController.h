/**
 * Created by Federico Manuel Gomez Peter 
 * on 23/12/20.
 */

#ifndef __HTTPCONTROLLER_H__
#define __HTTPCONTROLLER_H__

#include <cpprest/http_msg.h>

/**
 * Interfaz que debe cumplir cada uno de los callbacks de los endpoints que se registren en el servidor
 */
class HttpController {
public:
    HttpController() = default;
    virtual ~HttpController() = default;
    virtual void handleGet(web::http::http_request &message) = 0;
    virtual void handlePut(web::http::http_request &message) = 0;
    virtual void handlePost(web::http::http_request &message) = 0;
    virtual void handleDelete(web::http::http_request &message) = 0;
    virtual void handlePatch(web::http::http_request &messge) = 0;
    virtual void handleHead(web::http::http_request &message) = 0;
    virtual void handleOptions(web::http::http_request &message) = 0;
    virtual void handleTrace(web::http::http_request &message) = 0;
    virtual void handleConnect(web::http::http_request &message) = 0;
    virtual void handleMerge(web::http::http_request &message) = 0;
};


#endif //__HTTPCONTROLLER_H__
