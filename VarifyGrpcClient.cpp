#include "VarifyGrpcClient.h"

VarifyGrpcClient::VarifyGrpcClient()
{
	std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", 
		grpc::InsecureChannelCredentials());
	stub_ = VarifyService::NewStub(channel);
}


GetVarifyRsp VarifyGrpcClient::GetVarifyCode(const std::string& email)
{
	ClientContext context;
	GetVarifyReq request;
	GetVarifyRsp reply;
	request.set_email(email);

	Status status = stub_->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		return reply;
	} else {
		reply.set_error(ErrorCode::RPCFailed);
		return reply;
	}
}
