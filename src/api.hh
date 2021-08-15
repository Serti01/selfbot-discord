#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "../vendor/include/curlpp/cURLpp.hpp"
#include "../vendor/include/curlpp/Easy.hpp"
#include "../vendor/include/curlpp/Options.hpp"
#include "../vendor/include/curlpp/Infos.hpp"
#include "../vendor/json.hpp"
using nlohmann::json;

namespace bot {
    class net {
        private:

        public:
        std::string token;

        json discord(const std::string uri = "", std::string method = "GET", json body = "");

        net();
        ~net();
    };
};