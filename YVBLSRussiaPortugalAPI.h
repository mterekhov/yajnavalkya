//
//  YVAPI.hpp
//  yajnavalkya
//
//  Created by cipher on 03.09.2023.
//

#ifndef YVBLSRussiaPortugalAPI_H
#define YVBLSRussiaPortugalAPI_H

#include "YVHTTPRequestService.h"
#include "YVContentChecker.h"

namespace spcYajnaValkya {

class YVBLSRussiaPortugalAPI {
public:
    YVBLSRussiaPortugalAPI();
    ~YVBLSRussiaPortugalAPI();

    bool requestVerificationCode();
    bool requestAppointment(const std::string& otp);
    bool termsOfUseAgree();
    bool scheduleAppointment();

private:
    const std::string hostName = "blsrussiaportugal.com";
    std::string phpSession = "";
    std::string csrfToken = "";

    YVHTTPRequestService httpService;
    YVContentChecker checker;
    
    std::string parseCSRFToken(const std::string& response);
    std::string parsePHPSessionCookie(const std::string& response);
    HTTPParsType defaultHeaders();
    bool checkFreeSlots(const std::string& htmlBody);
};

};

#endif /* YVAPI_hpp */
