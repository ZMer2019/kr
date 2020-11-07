#include <iostream>

#include "crypto.h"
#include <grpc_client.h>
#include <http_server.h>
#include <cxxopts.hpp>
int main(int argc, char *argv[]) {
    std::string public_key = "";
    std::string private_key = "";

    cxxopts::Options options("KR","Description:");
    std::string default_addr = "10.10.17.123:9999";
    options.add_options()("s,srvaddr", "server addr", cxxopts::value<std::string>()->default_value(default_addr));


    auto result = options.parse(argc, argv);
    std::string target = result["s"].as<std::string>();

    bool bret = Crypto::GenerateKeyPair(&private_key, &public_key);
    if (!bret){
        std::cout << "Generate RSA key pairs error\n";
        exit(1);
    }
    HttpServer::private_key = private_key;
    std::cout << public_key <<"\n";
    std::cout << "grpc connect to "<< target <<"\n";
    std::shared_ptr<Channel> channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    GRPCClient client{channel};
    std::string cert = "";
    bret = client.IssueCert(public_key, &cert);
    if(!bret){
        std::cout << "Issue cert error\n";
        exit(1);
    }
    std::cout << "cert:" << cert;
    HttpServer::client_cert = cert;
    HttpServer server;
    std::cout << "http server start\n";
    server.Start();
    return 0;
}
