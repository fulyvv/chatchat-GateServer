#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"


using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;//上下文

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool 
{
public:
	RPConPool(std::size_t size, std::string host, std::string port);
	~RPConPool();
	std::unique_ptr<VarifyService::Stub> GetConnection();
	void ReturnConnection(std::unique_ptr<VarifyService::Stub> conn);
	void Close();

private:
	std::atomic<bool> _b_stop;
	size_t _poolSize;
	std::string _host;
	std::string _port;
	std::queue<std::unique_ptr<VarifyService::Stub>> _connections;
	std::mutex _mtx;
	std::condition_variable _cv;
};

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
	friend class Singleton<VarifyGrpcClient>;

public:
	GetVarifyRsp GetVarifyCode(const std::string& email);

private:
	VarifyGrpcClient();
	std::unique_ptr<VarifyService::Stub> stub_;
	std::unique_ptr<RPConPool> _pool;
};

