#include "VarifyGrpcClient.h"
#include "ConfigMgr.h"

RPConPool::RPConPool(std::size_t size, std::string host, std::string port):
	_poolSize(size), _host(host), _port(port), _b_stop(false)
{
	for (size_t i = 0; i < _poolSize; ++i) {
		std::shared_ptr<Channel> channel = grpc::CreateChannel(_host + ":" + _port,
			grpc::InsecureChannelCredentials());
		/*
		这种不行，这样就是拷贝构造了
		auto stub = VarifyService::NewStub(channel);
		_connections.push(std::move(stub));*/
		//这里push是移动构造move
		_connections.push(VarifyService::NewStub(channel));
	}
}

RPConPool::~RPConPool()
{
	std::lock_guard<std::mutex> lock(_mtx);
	Close();
	while (!_connections.empty()) {
		_connections.pop();
	}
}

std::unique_ptr<VarifyService::Stub> RPConPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(_mtx);
	_cv.wait(lock, [this]() { 
		if (_b_stop) return true;
		return !_connections.empty();
		});

	if (_b_stop) return nullptr;
	auto context = std::move(_connections.front());
	_connections.pop();
	return context;
}

void RPConPool::ReturnConnection(std::unique_ptr<VarifyService::Stub> context)
{
	std::lock_guard<std::mutex> lock(_mtx);
	if (_b_stop) return;
	_connections.push(std::move(context));
	_cv.notify_one();
}

void RPConPool::Close()
{
	_b_stop = true;
	_cv.notify_all();
}

VarifyGrpcClient::VarifyGrpcClient()
{
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyService"]["Host"];
	std::string port = gCfgMgr["VarifyService"]["Port"];
	_pool.reset(new RPConPool(5, host, port));
}


GetVarifyRsp VarifyGrpcClient::GetVarifyCode(const std::string& email)
{
	ClientContext context;
	GetVarifyReq request;
	GetVarifyRsp reply;
	request.set_email(email);
	auto stub = _pool->GetConnection();
	Status status = stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		_pool->ReturnConnection(std::move(stub));
		return reply;
	} else {
		_pool->ReturnConnection(std::move(stub));
		reply.set_error(ErrorCode::RPCFailed);
		return reply;
	}
}
