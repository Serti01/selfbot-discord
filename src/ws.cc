#include "ws.hh"
#include "websocketpp/common/connection_hdl.hpp"
#include "websocketpp/config/asio_client.hpp"
#include "websocketpp/message_buffer/message.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <type_traits>

void handle_discord_websock(json msg, wspp::connection_hdl hdl);

int count = 0;
client c;
bool started = false;
bot::net *net;

int get_count() {return count;}
client *get_c() {return &c;};
bool get_started() {return started;};

typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;

void send(wspp::connection_hdl hdl, json msg) {
    c.send(hdl, msg.dump(), wspp::frame::opcode::text);
}

static context_ptr tls_init_handle(wspp::connection_hdl hdl) {
    context_ptr ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds);
    } catch (std::exception& e) {
        std::cout <<"Error in context pointer: "<< e.what() << std::endl;
    }

    return ctx;
}

void set_net(bot::net *_net) {
    net = _net;
}
void init() {
    c.set_access_channels(wspp::log::alevel::all);
    c.clear_access_channels(wspp::log::alevel::frame_payload);
    c.set_error_channels(wspp::log::elevel::all);

    c.init_asio();
    c.start_perpetual();
    c.set_tls_init_handler(tls_init_handle);

    c.set_message_handler([&](wspp::connection_hdl hdl, client::message_ptr msg){
        std::cout << json::parse(msg->get_payload()).dump(2) << std::endl;
        handle_discord_websock(json::parse(msg->get_payload()),hdl);
    });

    wspp::lib::error_code ec;
    auto con = c.get_connection("wss://gateway.discord.gg/?encoding=json&v=9",ec);
    if (ec) {
        std::cerr << "Connection: " << ec.message();
        exit(1);
    }

    c.connect(con);
    started = true;
    c.run();
}

bool hb_end = false;
wspp::connection_hdl hb_hdl;
int hb_int = 0;
int hb_count = 0;
std::thread *hb_handle;

void handle_heartbeat(int heartbeat,wspp::connection_hdl hdl) {
    hb_int = heartbeat;
    hb_hdl = hdl;
}

void _hb_handle() {
    while (!hb_end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(hb_int));
        send(hb_hdl,{
            {"op",1},
            {"d",hb_count}
        });
    }
}


void handle_discord_websock(json msg, wspp::connection_hdl hdl) {
    if (msg["t"].type() == nlohmann::detail::value_t::null)
        switch (std::stoi(msg["op"].dump())) {
            case 1:{
                json pong = {
                    {"op",1}
                };
                send(hdl, pong.dump());
                break;
            }
            case 10:{
                // gotta do heartbeat stuff
                int heartbeat = msg["d"]["heartbeat_interval"];
                handle_heartbeat(heartbeat, hdl);

                // setup websock
                std::string ident = \
R"({"op": 2, "d": {
    "token": ")" + net->token + R"(",
    "capabilities": 125,
    "properties": {
        "os": "linux",
        "browser": "Firefox",
        "device": "",
        "system_locale": "en-US",
        "browser_user_agent": "Mozilla/5.0 (Linux) Gecko/20100101 Firefox/90.0",
        "browser_version": "90.0",
        "os_version": "",
        "referrer": "",
        "referring_domain": "",
        "referrer_current": "",
        "release_channel": "stable",
        "client_build_number": 94294,
        "client_event_source": null
    },
    "presence": {
        "status": "online",
        "since": 0,
        "activities": [],
        "afk": false
    },
    "compress": false,
    "client_state": {
        "guild_hashes": {},
        "highest_last_message_id": "0",
        "read_state_version": 0,
        "user_guild_settings_version": -1
    }
}})";

                // json iden = {
                //     {"d",{
                //         {"properties",{
                //             {"os","linux"},
                //             {"browser","samoyoiscool"},
                //             {"device","samoyoiscool"},
                //             {"system_locale","en-US"},
                //             {"referrer",""},
                //             {"referring_domain",""},
                //             {"release_channel","stable"}
                //         }},
                //         {"presence",{
                //             {"status","online"},
                //             {"since",0},
                //             {"activites",{}},
                //             {"afk",false}
                //         }},
                //         {"compress",false},
                //         {"token",net->token},
                //     }},
                //     {"op",2}
                // };
                std::cout << ident << std::endl;
                send(hdl,ident);
                break;
            }
            default: {
                std::cout << "Unhandled operation: " << msg["op"] << "\n" << msg.dump(2) << std::endl; 
                break;
            }
        }
    else {
        std::cout << "Unhandled operation: " << msg["t"] << "\n" << msg.dump(2) << std::endl;
    }
}
