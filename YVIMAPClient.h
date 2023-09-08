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
    YVIMAPClient(const std::string& host, const int port, const std::string& login, const std::string& password);
    ~YVIMAPClient();
    
    int fetchVerificationCode();
    
private:
    const std::string login;
    const std::string password;
    const std::string host;
    const int port;

    int fetchLastMessageIndex(const std::string& emailBody);
    bool checkEmailSubjectAndDate(const std::string& emailBody);
    std::string lastEmailBody();
};

};

#endif
