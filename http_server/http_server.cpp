//
// Created by edward yue on 2020/11/4.
//

#include "http_server.h"
#include <auth_message.pb.h>
using auth::Cookie;
using auth::AgentAuthInfo;
using auth::DeviceInfo;

std::string HttpServer::client_cert = "";
std::string HttpServer::private_key = "";

void HttpServer::Start() {
    //route
    httpServer_->Get("/auth", AuthHandler);

    //start http server
    if(!(httpServer_->listen("127.0.0.1", 52018))){
        std::cout << "listen 127.0.0.1:52018 error:" << strerror(errno)<<"\n";
        return;
    }

}
void HttpServer::AuthHandler(const httplib::Request &request, httplib::Response &response) {
    std::string msgStr = serialize();
    std::string signature = "";
    Crypto::Sign(private_key, msgStr, &signature);
    std::cout << signature <<"\n";
    std::string base64Str = "";
    Crypto::Base64Encode(msgStr, &base64Str);
    std::cout << "base64Str:" <<base64Str << "\n";
    std::string base64Sign = "";
    Crypto::Base64Encode(signature, &base64Sign);

    std::string base64Cert = "";
    Crypto::Base64Encode(client_cert, &base64Cert);

    std::string redirectUrl = request.get_param_value("oriurl");
    redirectUrl += "?authInfo=" + base64Str + "&sign=" + base64Sign + "&cert=" + base64Cert;
    std::cout << "rediret url:" << redirectUrl <<"\n";
    response.set_redirect(redirectUrl);
}

std::string HttpServer::serialize() {
    AgentAuthInfo agentAuthInfo;
    agentAuthInfo.set_did(1234);
    agentAuthInfo.set_token("token_test");
    DeviceInfo deviceInfo;
    std::string str = "";
    deviceInfo.set_sn("1234567890");
    deviceInfo.set_hostname("KR");
    deviceInfo.set_os("windows 10");
    deviceInfo.set_arch("windows 10");
    deviceInfo.set_version("1.0.0");
    deviceInfo.set_bios_id("qazwsx");
    //agentAuthInfo.set_allocated_info(&deviceInfo);
    *agentAuthInfo.mutable_info() = deviceInfo;


    Cookie *ck = agentAuthInfo.add_cookies();
    ck->set_name("test");
    ck->set_value("askdflkasjdfsald");
    ck->set_domain("oa.com");
    ck->set_path("/");
    ck->set_expire("2020-10-24");
    ck->set_maxage(864000000);
    ck->set_httponly(true);
    ck->set_secure(false);

    agentAuthInfo.SerializePartialToString(&str);
    std::cout << str << "\n";
    return str;
}
