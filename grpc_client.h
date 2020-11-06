//
// Created by edward yue on 2020/11/4.
//

#ifndef KR_GRPC_CLIENT_H
#define KR_GRPC_CLIENT_H
#include <grpcpp/grpcpp.h>
#include <issuecert.grpc.pb.h>
#include <memory>
using RSATest::CAServer;

using RSATest::IssueCertRequest;
using RSATest::IssueCertResponse;
using RSATest::Greeter;
using RSATest::HelloRequest;
using RSATest::HelloResponse;

using grpc::Channel;
class GRPCClient {
public:
    GRPCClient(std::shared_ptr<Channel> channel):
    issueCertStub_(CAServer::NewStub(channel)),helloStub_(Greeter::NewStub(channel)){}
    bool IssueCert(const std::string &public_key, std::string *cert);
    std::string SayHello(std::string msg);
private:
    std::unique_ptr<CAServer::Stub> issueCertStub_;
    std::unique_ptr<Greeter::Stub> helloStub_;
};


#endif //KR_GRPC_CLIENT_H
