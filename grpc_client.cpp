//
// Created by edward yue on 2020/11/4.
//

#include "grpc_client.h"

bool GRPCClient::IssueCert(const std::string &public_key, std::string *cert) {
    IssueCertRequest request;
    //gpr_timespec timespec;
    //timespec.tv_sec = 3;
    //timespec.tv_nsec = 0;
    //timespec.clock_type = GPR_TIMESPAN;
    request.set_public_key(public_key);
    IssueCertResponse response;
    grpc::ClientContext context;
    //context.set_deadline(timespec);
    grpc::Status status = issueCertStub_->IssueCert(&context, request, &response);
    if(status.ok()){
        *cert = response.cert();
        return true;
    }
    std::cout << status.error_message() << "\n";
    return false;
}
