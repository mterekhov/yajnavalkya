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
    printf("RESPONSE:\n %s\n", httpService.sendHTTPSRequest("/russian/book_appointment.php").c_str());
}

std::string YVBLSRussiaPortugalAPI::sendVerificationCode(const std::string& verificationCode) {
    
}

};
