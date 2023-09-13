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

enum EScheduleRequestType {
    SCHEDULE_REQUEST_FAMILY,
    SCHEDULE_REQUEST_SINGLE
};

class YVBLSRussiaPortugalAPI {
public:
    YVBLSRussiaPortugalAPI();
    ~YVBLSRussiaPortugalAPI();

    bool requestVerificationCode();
    bool requestAppointment(EScheduleRequestType type, const std::string& otp);
    bool termsOfUseAgree(EScheduleRequestType type);
    bool scheduleAppointment(const EScheduleRequestType type);

private:
    const std::string hostName = "blsrussiaportugal.com";
    std::string phpSession = "";
    std::string csrfToken = "";

    YVHTTPRequestService httpService;
    YVContentChecker checker;
    
    HTTPParsType parametersCombination(const EScheduleRequestType type, const std::string& otp);
    std::string parseCSRFToken(const std::string& response);
    std::string parsePHPSessionCookie(const std::string& response);
    HTTPParsType defaultHeaders();
    bool checkFreeSlots(const std::string& htmlBody);
};

};

#endif /* YVAPI_hpp */
