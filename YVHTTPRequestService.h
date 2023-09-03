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
#include <list>
#include <openssl/ssl.h>

#include "YVHTTPRequest.h"

namespace spcYajnaValkya {

class YVHTTPRequestService {
public:
    
    YVHTTPRequestService(const std::string& host);
    ~YVHTTPRequestService();
    
    std::string sendHTTPSRequest(const std::string& link);

private:
    const std::string hostName;
    std::string csrfToken;
    
    int connectToServer(const std::string hostAddress);
    SSL *enableSSL(const int socket);
};

};

#endif
