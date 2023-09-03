//
//  YVHTTPRequestService.h
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#ifndef YVHTTPRequestService_h
#define YVHTTPRequestService_h

#include <stdio.h>
#include <string>
#include <map>
#include <list>
#include <openssl/ssl.h>

#include "YVHTTPRequest.h"

namespace spcYajnaValkya {

class YVHTTPPar {
public:
    std::string key;
    std::string value;
};

using HTTPParsType = std::list<YVHTTPPar>;

class YVHTTPRequestService {
public:
    
    YVHTTPRequestService(const std::string& host);
    ~YVHTTPRequestService();
    
    std::string sendGETRequest(const std::string& link);
    std::string sendPOSTRequest(const std::string& link, const HTTPParsType& pars);

private:
    const std::string hostName;
    std::string csrfToken;
    
    std::string generateBody(const HTTPParsType& parsList);
    int connectToServer(const std::string hostAddress);
    SSL *enableSSL(const int socket);
    std::string sendHTTPSRequest(const std::string& method,
                                 const std::string& link,
                                 const HTTPParsType& pars);
};

};

#endif
