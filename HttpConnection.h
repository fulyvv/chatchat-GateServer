#pragma once
#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(tcp::socket socket);
	void Start();
private:
	//实现超时检测函数
	void CheckDeadline();
	//应答函数
	void WriteResponse();
	//处理请求
	void HandleReq();
	void PreParseGetParam();
	tcp::socket _socket;
	beast::flat_buffer _buffer{8192}; //8k缓冲区
	http::request<http::dynamic_body> _request; //请求对象
	http::response<http::dynamic_body> _response; //应答对象
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60)
	}; //定时器,初始化构造

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

