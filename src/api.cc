#include "api.hh"

namespace {
    std::string data;
    char *dataBuffer = NULL;
    size_t m_size = 0;

    size_t writeFunc(char *ptr, size_t size, size_t nmemb){
        size_t realsize = size * nmemb;

        if (ptr)
            dataBuffer = (char*)realloc(ptr,m_size+realsize);
        else
            dataBuffer = (char*)malloc(m_size+realsize);

        if (dataBuffer == NULL)
            realsize = 0;

        memcpy(&(dataBuffer[m_size]), ptr, realsize);
        m_size += realsize;

        return realsize;
    }
};

json bot::net::discord(const std::string uri, std::string method, json body) {
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

        req.setOpt<CustomRequest>(method);

        std::cout << "-" << body << "-" << std::endl;

        if (body != "")
        {
            headers.push_back("Content-Length: "+std::to_string(body.dump().length()));
            req.setOpt(new InfileSize(body.dump().length()));
            req.setOpt(new Upload(true));
        }

        req.setOpt<Verbose>(false);
        req.setOpt<HttpHeader>(headers);
        req.setOpt<ReadFunction>([body](char *buffer, size_t size, size_t nitems){
            strncpy(buffer, body.dump().c_str(), size * nitems);
            return size * nitems;
        });
        req.setOpt<Url>("https://discord.com/api/v9/"+uri);

        std::stringstream ss;
        ss << req;

        long status = curlpp::Infos::ResponseCode::get(req);
        if (status)
            printf("API (%s) error %lu", uri.c_str(), status);

        return json::parse(ss.str());
    }
    catch(curlpp::RuntimeError &e) {
        std::cerr << e.what() << std::endl;
    }
    catch(curlpp::LogicError &e) {
        std::cerr << e.what() << std::endl;
    }

    return NULL;
}

bot::net::net() {

}
bot::net::~net() {

}