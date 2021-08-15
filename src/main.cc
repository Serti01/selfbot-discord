#include "main.hh"

bool ended = false;

int main() {
    curlpp::initialize();
    bot::net net;

    if (getenv("TOKEN") == NULL)
        exit(1);

    net.token = getenv("TOKEN");

    // TODO: Communicate with discord websocket
    std::thread wsock(init);
    wsock.detach();
    
    while (!get_started())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    while (!ended)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    curlpp::terminate();
}