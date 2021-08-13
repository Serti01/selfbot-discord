#include "main.hh"
#include "curlpp/cURLpp.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
    curlpp::initialize();
    bot::discord::api api;
    api.token = secure_getenv("DISCORD_TOKEN");
    std::cout << "\nSending /v9/users/@me\n" << std::endl;
    json test = api.send("users/@me");
    std::cout << "\n A \n";
    std::cout << test.dump(2) << std::endl;
    curlpp::terminate();
}