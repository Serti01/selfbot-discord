#pragma once

#include <SFML/Network.hpp>
#include "../vendor/json.hpp"

using nlohmann::json;

namespace bot {
    namespace discord {
        class api {
            private:
            sf::Http::Response wrapper(sf::Http::Request req);

            public:
            std::string token;
            sf::Http::Response GET(const std::string uri = "/", const std::string body = "");
            sf::Http::Response POST(const std::string uri = "/", const std::string body = "");
            sf::Http::Response DELETE(const std::string uri = "/", const std::string body = "");
            sf::Http::Response GET(const std::string uri = "/", json json = {});
            sf::Http::Response POST(const std::string uri = "/", json json = {});
            sf::Http::Response DELETE(const std::string uri = "/", json json = {});

            api();
            ~api();
        };
    };
};