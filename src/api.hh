#pragma once

#include "../vendor/include/curlpp/cURLpp.hpp"
#include "../vendor/include/curlpp/Easy.hpp"
#include "../vendor/include/curlpp/Options.hpp"
#include "../vendor/include/curlpp/Infos.hpp"
#include "main.hh"

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