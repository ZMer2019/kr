//
// Created by edward yue on 2020/11/4.
//

#ifndef KR_GRPC_CLIENT_H
#define KR_GRPC_CLIENT_H
#include <grpcpp/grpcpp.h>
#include <issuecert.grpc.pb.h>
#include <memory>
using auth::CAServer;

using auth::IssueCertRequest;
using auth::IssueCertResponse;

using grpc::Channel;
class GRPCClient {
public:
    GRPCClient(std::shared_ptr<Channel> channel):
    issueCertStub_(CAServer::NewStub(channel)){}
    bool IssueCert(const std::string &public_key, std::string *cert);
private:
    std::unique_ptr<CAServer::Stub> issueCertStub_;
};


#endif //KR_GRPC_CLIENT_H
