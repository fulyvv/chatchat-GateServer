#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CServer.h"
#include "ConfigMgr.h"

int main()
{
	ConfigMgr gCfgMgr; //全局配置管理器
	std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
	unsigned short gate_port = atoi(gate_port_str.c_str());
    try
    {
		unsigned short port = static_cast<unsigned short>(8080); //端口号
		net::io_context ioc{ 1 }; //单线程
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop();
        });
        auto server = std::make_shared<CServer>(ioc, port);
        server->Start();
		std::cout << "Server started at port: " << port << std::endl;
        ioc.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}