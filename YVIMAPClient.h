//
//  YVVerificationCode.hpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#ifndef YVIMAPClient_h
#define YVIMAPClient_h

#include <stdio.h>
#include <string>

namespace spcYajnaValkya {

class YVIMAPClient {
public:
    int fetchVerificationCode();
    
private:
    int fetchLastMessageIndex(const std::string& emailBody);
    bool checkEmailSubjectAndDate(const std::string& emailBody);
    std::string lastEmailBody();
};

};

#endif
