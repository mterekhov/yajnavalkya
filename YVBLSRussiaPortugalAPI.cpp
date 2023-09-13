//
//  YVAPI.cpp
//  yajnavalkya
//
//  Created by cipher on 03.09.2023.
//

#include "YVBLSRussiaPortugalAPI.h"
#include "YVTools.h"

namespace spcYajnaValkya {

YVBLSRussiaPortugalAPI::YVBLSRussiaPortugalAPI() : httpService(YVHTTPRequestService(hostName)) {
    
}

YVBLSRussiaPortugalAPI::~YVBLSRussiaPortugalAPI() {
    
}

bool YVBLSRussiaPortugalAPI::scheduleAppointment() {
    HTTPHeadersType headers = defaultHeaders();
    if (!phpSession.empty()) {
        headers.push_back({"Cookie", "PHPSESSID=" + phpSession});
    }
    std::string response = httpService.sendGETRequest("/russian/appointment_family.php", HTTPParsType(), headers);
    if (!checker.checkStep_05(response)) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: page with available dates is corrupted\n");
        return false;
    }
    return checkFreeSlots(response);
}

bool YVBLSRussiaPortugalAPI::checkFreeSlots(const std::string& htmlBody) {
    std::string freeSlots = YVTools::parse("var available_dates = [",
                                           "]",
                                           htmlBody);
    if (freeSlots.empty()) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: no free time slots found\n");
        return false;
    }
    
    return true;
}

bool YVBLSRussiaPortugalAPI::termsOfUseAgree(EScheduleRequestType type) {
    HTTPParsType headers = defaultHeaders();
    if (!phpSession.empty()) {
        headers.push_back({"Cookie", "PHPSESSID=" + phpSession});
    }
    std::string response = httpService.sendPOSTRequest("/russian/book_appointment.php", {
        {"agree", "Agree"}
    },
                                                       headers);
    FILE *filo = fopen("/Users/cipher/agreement_result.html", "wb");
    fwrite(response.c_str(), response.length(), 1, filo);
    fclose(filo);
    
    switch (type) {
        case SCHEDULE_REQUEST_SINGLE:
            if (!checker.checkStep_04_single(response)) {
                YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: failed to sign agreement because response is corrupted\n");
                return false;
            }
            break;
        case SCHEDULE_REQUEST_FAMILY:
            if (!checker.checkStep_04(response)) {
                YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: failed to sign agreement because response is corrupted\n");
                return false;
            }
            break;
    }
    
    phpSession = parsePHPSessionCookie(response);
    return true;
}

bool YVBLSRussiaPortugalAPI::requestAppointment(EScheduleRequestType type, const std::string& otp) {
    HTTPParsType headers = defaultHeaders();
    if (!phpSession.empty()) {
        headers.push_back({"Cookie", "PHPSESSID=" + phpSession});
    }
    std::string response = httpService.sendPOSTRequest("/russian/book_appointment.php",
                                                       parametersCombination(type, otp),
                                                       headers);
    FILE *filo = fopen("/Users/cipher/agreement.html", "wb");
    fwrite(response.c_str(), response.length(), 1, filo);
    fclose(filo);
    if (!checker.checkStep_03(response)) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: agreement page is corrupted\n");
        return false;
    }
    
    phpSession = parsePHPSessionCookie(response);
    return true;
}

bool YVBLSRussiaPortugalAPI::requestVerificationCode() {
    std::string response = httpService.sendGETRequest("/russian/book_appointment.php");
    if (!checker.checkStep_01(response)) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: initial html page is corrupted\n");
        return false;
    }
    
    //  fetch CSRF token
    csrfToken = parseCSRFToken(response);
    if (csrfToken.empty()) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: CSRF token not found\n");
        return false;
    }
    
    //  fetch PHPSESSID cookie
    phpSession = parsePHPSessionCookie(response);
    HTTPParsType headers = defaultHeaders();
    if (!phpSession.empty()) {
        headers.push_back({"Cookie", "PHPSESSID=" + phpSession});
    }
    
    //  request for OTP on email
    response = httpService.sendPOSTRequest("/russian/ajax.php", {
        {"gofor", "send_mail_for_vac"},
        {"email", "yajnavalkya@disroot.org"},
        {"center_id", "1"},
        {"token", csrfToken}
    },
                                           headers);
    if (!checker.checkStep_02(response)) {
        YVTools::vidyaInfo("YVBLSRussiaPortugalAPI: request of verification code is failed\n");
        return false;
    }
    
    //  fetch PHPSESSID cookie
    phpSession = parsePHPSessionCookie(response);
    return true;
}

HTTPParsType YVBLSRussiaPortugalAPI::parametersCombination(const EScheduleRequestType type, const std::string& otp) {
    HTTPParsType requestPars;
    requestPars.push_back({"centre", "6%231"});
    requestPars.push_back({"category", "Normal"});
    requestPars.push_back({"phone_code", "7"});
    requestPars.push_back({"phone", "9160861355"});
    requestPars.push_back({"email", "yajnavalkya%40disroot.org"});
    requestPars.push_back({"otp", otp});
    requestPars.push_back({"countryID", ""});
    requestPars.push_back({"tokenvalue", csrfToken});
    requestPars.push_back({"save", "%D0%9F%D1%80%D0%BE%D0%B4%D0%BE%D0%BB%D0%B6%D0%B8%D1%82%D1%8C"});
    
    switch (type) {
        case SCHEDULE_REQUEST_FAMILY:
            requestPars.push_back({"app_type", "Family"});
            requestPars.push_back({"member", "3"});
            break;
            
        case SCHEDULE_REQUEST_SINGLE:
            requestPars.push_back({"app_type", "Individual"});
            requestPars.push_back({"member", "2"});
            break;
    }
    
    return requestPars;
}

std::string YVBLSRussiaPortugalAPI::parseCSRFToken(const std::string& response) {
    return YVTools::parse("<input type=\"hidden\" name=\"tokenvalue\" id=\"csrftokenvalue\" value=\"",
                          "\">",
                          response);
}

std::string YVBLSRussiaPortugalAPI::parsePHPSessionCookie(const std::string& response) {
    std::string startMark = "Set-Cookie: PHPSESSID=";
    std::string token = YVTools::parse(startMark,
                                       "; path=/;",
                                       response);
    if (token.empty()) {
        YVTools::vidyaWarning("YVBLSRussiaPortugalAPI: PHPSESSID token was not found\n");
    }
    
    return token;
}

HTTPParsType YVBLSRussiaPortugalAPI::defaultHeaders() {
    return {
        {"Content-Type", "application/x-www-form-urlencoded; charset=UTF-8"},
        {"Accept", "*/*"},
        {"Sec-Fetch-Site", "same-origin"},
        {"Accept-Language", "en-GB,en;q=0.9"},
        {"Accept-Encoding", "gzip, deflate, br"},
        {"Sec-Fetch-Mode", "cors"},
        {"Origin", "https://blsrussiaportugal.com"},
        {"User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.5.2 Safari/605.1.15"},
        {"Referer", "https://blsrussiaportugal.com/russian/book_appointment.php"},
        {"Connection", "keep-alive"},
        {"Sec-Fetch-Dest", "empty"},
        {"X-Requested-With", "XMLHttpRequest"},
    };
}

};
