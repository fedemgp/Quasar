/**
 * Created by Federico Manuel Gomez Peter 
 * on 23/12/20.
 */

#ifndef __TOPSECRETCONTROLLER_H__
#define __TOPSECRETCONTROLLER_H__

#include "HttpController.h"
#include "Decoder.h"

class TopSecretController: public HttpController {
private:
    Decoder &decoder;
public:
    TopSecretController(Decoder &dec);
    ~TopSecretController();
    void handleGet(web::http::http_request &message) override;
    void handlePut(web::http::http_request &message) override;
    void handlePost(web::http::http_request &message) override;
    void handleDelete(web::http::http_request &message) override;
    void handlePatch(web::http::http_request &messge) override;
    void handleHead(web::http::http_request &message) override;
    void handleOptions(web::http::http_request &message) override;
    void handleTrace(web::http::http_request &message) override;
    void handleConnect(web::http::http_request &message) override;
    void handleMerge(web::http::http_request &message) override;
};


#endif //__TOPSECRETCONTROLLER_H__
