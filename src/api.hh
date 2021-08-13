#pragma once

#include "../vendor/include/curlpp/cURLpp.hpp"
#include "../vendor/include/curlpp/Easy.hpp"
#include "../vendor/include/curlpp/Options.hpp"
#include "../vendor/json.hpp"

using nlohmann::json;

namespace bot {
    namespace discord {
        class api {
            private:

            public:
            enum method{
                GET,
                POST,
                DELETE,
            };
            std::string token;

            json send(const std::string uri = "", bot::discord::api::method method = GET, json body = NULL);

            api();
            ~api();
        };
    };
};