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

void YVBLSRussiaPortugalAPI::receiveVerificationCode() {
//    gofor=send_mail_for_vac&email=m.terekhov@icloud.com&center_id=1&token=eaa32c96f620053cf442ad32258076b9
    std::string initialStepResponse = httpService.sendGETRequest("/russian/book_appointment.php");
    printf("RESPONSE:\n%s\n", initialStepResponse.c_str());
    std::string csrfToken = parseCSRFToken(initialStepResponse);
    if (csrfToken.empty()) {
        printf(">>> CSRF token not found\n");
        return;
    }
    
    printf("RESPONSE:\n%s\n", httpService.sendPOSTRequest("/russian/ajax.php", {
        {"gofor", "send_mail_for_vac"},
        {"email", "yajnavalkya@disroot.org"},
        {"center_id", "1"},
        {"token", csrfToken}
    }).c_str());
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

};
