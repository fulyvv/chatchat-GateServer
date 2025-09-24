﻿#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):
	_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{

}

void CServer::Start()
{
	auto self = shared_from_this(); //生成一个shared_ptr对象
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
			try {
				//出错放弃当前连接，继续监听其他连接
				if (ec){
					self->Start();
					return;
				}

				//创建新连接，并且创建HttpConnection类管理这个连接
				//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
				new_con->Start();
				//继续监听其他连接
				self->Start();
			}
			catch (std::exception& exp){
				std::cout << "accept exception:" << exp.what() << std::endl;
			}
		});

}
