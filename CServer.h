#pragma once
#include "const.h"
#include "AsioIOServicePool.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start(); //服务器启动
private:
	tcp::acceptor _acceptor; //监听器
	net::io_context& _ioc;
};

