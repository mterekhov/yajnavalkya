//
//  YVVerificationCode.hpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#ifndef YVVerificationCode_h
#define YVVerificationCode_h

#include <stdio.h>
#include <string>

namespace spcYajnaValkya {

class YVVerificationCode {
public:
    int fetchVerificationCode();
    
private:
    int fetchLastMessageIndex(const std::string& emailBody);
    bool checkEmailSubjectAndDate(const std::string& emailBody);
    std::string lastEmailBody();
};

};

#endif
