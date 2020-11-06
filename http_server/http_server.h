//
// Created by edward yue on 2020/11/4.
//

#ifndef KR_HTTP_SERVER_H
#define KR_HTTP_SERVER_H
#include <memory>
#include "httplib.h"
#include <crypto.h>


class HttpServer {
public:
    HttpServer(){
        httpServer_.reset(new httplib::Server());
    }
    ~HttpServer(){
        httpServer_->stop();
    }
    void Start();

private:
    static void AuthHandler(const httplib::Request &request, httplib::Response &response);
    static std::string serialize(void);
private:
    std::unique_ptr<httplib::Server> httpServer_;
};


#endif //KR_HTTP_SERVER_H
