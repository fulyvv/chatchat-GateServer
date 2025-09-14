#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/reader.h>

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCode {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002,
};