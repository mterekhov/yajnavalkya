//
//  YVAPI.hpp
//  yajnavalkya
//
//  Created by cipher on 03.09.2023.
//

#ifndef YVBLSRussiaPortugalAPI_H
#define YVBLSRussiaPortugalAPI_H

#include "YVHTTPRequestService.h"

namespace spcYajnaValkya {

class YVBLSRussiaPortugalAPI {
private:
    const std::string hostName = "blsrussiaportugal.com";
    std::string phpSession = "";

    YVHTTPRequestService httpService;
    
    std::string parseCSRFToken(const std::string& response);
    std::string parsePHPSessionCookie(const std::string& response);
    void waitFor(const int seconds);
    HTTPParsType defaultHeaders();

public:
    YVBLSRussiaPortugalAPI();
    ~YVBLSRussiaPortugalAPI();

    void receiveVerificationCode();
    std::string sendVerificationCode(const std::string& verificationCode);
};

};

#include <stdio.h>

#endif /* YVAPI_hpp */
