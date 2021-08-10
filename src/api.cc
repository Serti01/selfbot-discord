#include "api.hh"

sf::Http::Response bot::discord::api::wrapper(sf::Http::Request req) {
    sf::Http http;
    http.setHost("discord.com");
    
    req.setField("User-Agent", "Mozilla/5.0 (Linux) Gecko/20100101 Firefox/90.0");
    req.setField("Accept","*/*");
    req.setField("Accept-Language","en-US");
    if (this->token.length() != 0)
        req.setField("Authorization", this->token);
    else
        req.setField("Authorization", "undefined");

    sf::Http::Response res = http.sendRequest(req, sf::seconds(15));
    if (res.getStatus() != res.Ok || res.getStatus() != res.NoContent) {
        std::printf("Api request failed with %i\n", res.getStatus());
    }

    return res;
};

sf::Http::Response bot::discord::api::GET(const std::string uri, const std::string body) {
    sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Get, body);
    req.setField("Content-Type", "application/x-www-form-urlencoded");
    return wrapper(req);
}
sf::Http::Response bot::discord::api::POST(const std::string uri, const std::string body) {
    sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Post, body);
    req.setField("Content-Type", "application/x-www-form-urlencoded");
    return wrapper(req);
}
sf::Http::Response bot::discord::api::DELETE(const std::string uri, const std::string body) {
    sf::Http::Request req("/api/v9/"+uri, sf::Http::Request::Method::Delete, body);
    req.setField("Content-Type", "application/x-www-form-urlencoded");
    return wrapper(req);
};
sf::Http::Response bot::discord::api::GET(const std::string uri, json json) {
    sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Get, json.get<std::string>());
    req.setField("Content-Type", "application/json");
    return wrapper(req);
}
sf::Http::Response bot::discord::api::POST(const std::string uri, json json) {
    sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Post, json.get<std::string>());
    req.setField("Content-Type", "application/json");
    return wrapper(req);
}
sf::Http::Response bot::discord::api::DELETE(const std::string uri, json json) {
    sf::Http::Request req("/api/v9/"+uri,sf::Http::Request::Method::Delete, json.get<std::string>());
    req.setField("Content-Type", "application/json");
    return wrapper(req);
}

bot::discord::api::api() {
    
}
bot::discord::api::~api() {

}