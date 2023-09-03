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

    YVHTTPRequestService httpService;
    
public:
    YVBLSRussiaPortugalAPI();
    ~YVBLSRussiaPortugalAPI();

    void receiveVerificationCode();
    std::string sendVerificationCode(const std::string& verificationCode);
};

};

#include <stdio.h>

#endif /* YVAPI_hpp */
