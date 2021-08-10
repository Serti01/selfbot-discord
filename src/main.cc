#include "main.hh"
#include <cstdio>
#include <iostream>

int main() {
    bot::discord::api api;
    api.token = "";
    sf::Http::Response res = api.GET("users/@me", (std::string)"");
    std::cout << res.getBody() << " " << res.getField("Server");
}