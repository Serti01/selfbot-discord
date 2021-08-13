#include "api.hh"
#include "curlpp/Exception.hpp"
#include "curlpp/Options.hpp"
#include "curlpp/Types.hpp"
#include "curlpp/cURLpp.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>


// sf::Http::Response bot::discord::api::GET(const std::string uri, const std::string body) {
//     sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Get, body);
//     req.setField("Content-Type", "application/x-www-form-urlencoded");
//     return wrapper(req);
// }
// sf::Http::Response bot::discord::api::POST(const std::string uri, const std::string body) {
//     sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Post, body);
//     req.setField("Content-Type", "application/x-www-form-urlencoded");
//     return wrapper(req);
// }
// sf::Http::Response bot::discord::api::DELETE(const std::string uri, const std::string body) {
//     sf::Http::Request req("/api/v9/"+uri, sf::Http::Request::Method::Delete, body);
//     req.setField("Content-Type", "application/x-www-form-urlencoded");
//     return wrapper(req);
// };
// sf::Http::Response bot::discord::api::GET(const std::string uri, json json) {
//     sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Get, json.get<std::string>());
//     req.setField("Content-Type", "application/json");
//     return wrapper(req);
// }
// sf::Http::Response bot::discord::api::POST(const std::string uri, json json) {
//     sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Post, json.get<std::string>());
//     req.setField("Content-Type", "application/json");
//     return wrapper(req);
// }
// sf::Http::Response bot::discord::api::DELETE(const std::string uri, json json) {
//     sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Delete, json.get<std::string>());
//     req.setField("Content-Type", "application/json");
//     return wrapper(req);
// }

// sf::Http::Response bot::discord::api::wrapper(sf::Http::Request req) {
//     sf::Http http;
//     http.setHost("discord.com");
    


//     sf::Http::Response res = http.sendRequest(req, sf::seconds(15));
//     if (res.getStatus() != res.Ok || res.getStatus() != res.NoContent) {
//         std::printf("Api request failed with %i\n", res.getStatus());
//     }

//     return res;
// };


namespace {
    char *data = NULL;

    size_t readData(char *buffer, size_t size, size_t nitems)
    {
        strncpy(buffer, data, size * nitems);
        return size * nitems;
    }
}

json bot::discord::api::send(const std::string uri, bot::discord::api::method method, json body) {
    using namespace curlpp::options;
    
    try {
        curlpp::Easy req;
        
        std::list<std::string> headers;
        headers.push_back("User-Agent: Mozilla/5.0 (Linux) Gecko/20100101 Firefox/90.0");
        headers.push_back("Accept: */*");
        headers.push_back("Accept-Language: en-US");
        headers.push_back("Authorization: "+this->token);
        headers.push_back("DNT: 1");
        headers.push_back("Content-Type: application/json");
    
        req.setOpt<Verbose>(false);
        req.setOpt<ReadFunction>(curlpp::types::ReadFunctionFunctor(readData));
        req.setOpt<HttpHeader>(headers);
        req.setOpt<Url>("https://discord.com/api/v9/"+uri);

        req.perform();
    
        return data;
    }
    catch(curlpp::RuntimeError &e) {
        std::cerr << e.what() << std::endl;
    }
    catch(curlpp::LogicError &e) {
        std::cerr << e.what() << std::endl;
    }

    return NULL;
}

bot::discord::api::api() {

}
bot::discord::api::~api() {

}