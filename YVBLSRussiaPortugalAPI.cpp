//
//  YVAPI.cpp
//  yajnavalkya
//
//  Created by cipher on 03.09.2023.
//

#include "YVBLSRussiaPortugalAPI.h"

namespace spcYajnaValkya {

YVBLSRussiaPortugalAPI::YVBLSRussiaPortugalAPI() : httpService(YVHTTPRequestService(hostName)) {
    
}

YVBLSRussiaPortugalAPI::~YVBLSRussiaPortugalAPI() {
    
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


void YVBLSRussiaPortugalAPI::requestVerificationCode() {
    HTTPParsType headers = defaultHeaders();
    std::string response = httpService.sendGETRequest("/russian/book_appointment.php");
    printf("RESPONSE:\n%s\n", response.c_str());
    std::string csrfToken = parseCSRFToken(response);
    if (csrfToken.empty()) {
        printf(">>> CSRF token not found\n");
        return;
    }
    phpSession = parsePHPSessionCookie(response);
    if (!phpSession.empty()) {
        headers = defaultHeaders();
        headers.push_back({"Cookie", "PHPSESSID=" + phpSession});
    }

    //  request for OTP on email
    printf("//====================================================\n");
    response = httpService.sendPOSTRequest("/russian/ajax.php", {
        {"gofor", "send_mail_for_vac"},
        {"email", "yajnavalkya@disroot.org"},
        {"center_id", "1"},
        {"token", csrfToken}
    }, headers);
    printf("RESPONSE:\n%s\n", response.c_str());
}

std::string YVBLSRussiaPortugalAPI::sendVerificationCode(const std::string& verificationCode) {
    
}

std::string YVBLSRussiaPortugalAPI::parseCSRFToken(const std::string& response) {
    std::string startMark = "<input type=\"hidden\" name=\"tokenvalue\" id=\"csrftokenvalue\" value=\"";
    std::string endMark = "\">";
    auto startPosition = response.find(startMark);

    auto markStartPosition = response.find(startMark);
    auto markEndPosition = response.find(endMark, markStartPosition);
    std::string token = response.substr(markStartPosition + startMark.length(), markEndPosition - markStartPosition - startMark.length());

    return token;
}

std::string YVBLSRussiaPortugalAPI::parsePHPSessionCookie(const std::string& response) {
    std::string startMark = "Set-Cookie: PHPSESSID=";
    std::string endMark = "; path=/;";
    auto startPosition = response.find(startMark);

    auto markStartPosition = response.find(startMark);
    auto markEndPosition = response.find(endMark, markStartPosition);
    std::string token = response.substr(markStartPosition + startMark.length(), markEndPosition - markStartPosition - startMark.length());

    return token;
}

};
