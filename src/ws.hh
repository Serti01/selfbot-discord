#pragma once

#include "../vendor/include/websocketpp/config/asio_client.hpp"
#include "../vendor/include/websocketpp/config/core_client.hpp"
#include "../vendor/include/websocketpp/client.hpp"
#include <list>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../vendor/json.hpp"
using nlohmann::json;
#include "api.hh"

namespace wspp  = websocketpp;

typedef wspp::client<wspp::config::asio_tls_client> client;

void set_net(bot::net *_api);
void init();
void send(json msg);
int get_count();
client *get_c();
bool get_started();


// namespace bot {
//     class ws {
//         private:

//         public:
//     };
// };