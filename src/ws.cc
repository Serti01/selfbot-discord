#include "ws.hh"
#include "websocketpp/message_buffer/message.hpp"


int count = 0;
client c;
client::connection_ptr con;
bool started = false;
std::list<json> msgs;

int get_count() {return count;}
client *get_c() {return &c;};
client::connection_ptr get_con() {return con;};
bool get_started() {return started;};
std::list<json> get_msgs() {return msgs;};

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
void init() {
    c.set_access_channels(wspp::log::alevel::all);
    c.clear_access_channels(wspp::log::alevel::frame_payload);
    c.set_error_channels(wspp::log::elevel::all);

    c.init_asio();
    c.set_tls_init_handler(tls_init_handle);

    c.set_message_handler([&](wspp::connection_hdl hdl, client::message_ptr msg){
        std::cout << json::parse(msg->get_payload()).dump(2) << std::endl;
        msgs.push_back(json::parse(msg->get_payload()));
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