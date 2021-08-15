#include "ws.hh"
#include "websocketpp/message_buffer/message.hpp"
#include <string>
#include <type_traits>

void handle_discord_websock(json msg);

int count = 0;
client c;
client::connection_ptr con;
bool started = false;
bot::net *net;

int get_count() {return count;}
client *get_c() {return &c;};
client::connection_ptr get_con() {return con;};
bool get_started() {return started;};

typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;

void send(json msg) {
    con->send(msg.dump());
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
    c.set_tls_init_handler(tls_init_handle);

    c.set_message_handler([&](wspp::connection_hdl hdl, client::message_ptr msg){
        std::cout << json::parse(msg->get_payload()).dump(2) << std::endl;
        handle_discord_websock(json::parse(msg->get_payload()));
    });

    wspp::lib::error_code ec;
    con = c.get_connection("wss://gateway.discord.gg/?encoding=json&v=9",ec);
    if (ec) {
        std::cerr << "Connection: " << ec.message();
        exit(1);
    }

    c.connect(con);
    started = true;
    c.run();
}


void handle_discord_websock(json msg) {
    if (msg["t"].type() == nlohmann::detail::value_t::null)
        switch (std::stoi(msg["op"].dump())) {
            case 10:{
                // setup websock
                std::string token = net->token;
                json ret;
                ret.parse("{ \"op\": 2, \"d\": {\
        \"token\": \""+token+"\", \"capabilities\": 125, \"properties\": {\
            \"os\": \"linux\",\"browser\":\"Firefox\", \"device\":\"\", \"system_locale\": \"en-US\",\
            \"browser_user_agent\": \"Mozilla/5.0 (Linux) Gecko/20100101 Firefox/90.0\", \"browser_version\":\"90.0\",\
            \"os_version\":\"\", \"referrer\":\"\", \"referring_domain\":\"\", \"referrer_current\":\"\",\
            \"release_channel\":\"stable\", \"client_build_number\":93554,\"client_event_source\":null\
        },\
        \"presence\":{\
            \"status\":\"online\", \"since\":0, \"activities\":[], \"afk\":false\
        },\
        \"compress\":false,\
        \"client_state\":{\
            \"guild_hashes\":{}, \"highest_last_message_id\":\"0\", \"read_state_version\":0, \"user_guild_settings_version\":-1\
        }\
    }}");
                send(ret.dump());
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