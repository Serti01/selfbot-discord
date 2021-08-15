#pragma once

#include "../vendor/include/websocketpp/config/asio_client.hpp"
#include "../vendor/include/websocketpp/config/core_client.hpp"
#include "../vendor/include/websocketpp/client.hpp"
#include <list>
#include "main.hh"

namespace wspp  = websocketpp;

typedef wspp::client<wspp::config::asio_tls_client> client;

void init();
void send(json msg);
int get_count();
client *get_c();
bool get_started();
std::list<json> get_msgs();


// namespace bot {
//     class ws {
//         private:

//         public:
//     };
// };