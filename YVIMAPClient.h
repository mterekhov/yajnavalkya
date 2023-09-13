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
#include <openssl/bio.h>
#include <time.h>

namespace spcYajnaValkya {

class YVIMAPClient {
public:
    YVIMAPClient(const std::string& host, const int port, const std::string& login, const std::string& password, const time_t timeArea);
    ~YVIMAPClient();
    
    std::string fetchVerificationCode();
    
private:
    const std::string login;
    const std::string password;
    const std::string host;
    const int port;
    
    //  when the last email body found it will be checked if it's date/time is not exceeding this value in seconds
    const time_t timeArea;
    
    //  period of checking for email in seconds
    time_t refreshPeriod = 30;

    std::string sendRequest(const std::string& request, BIO *bio);
    std::string fetchOTP(const std::string emailBody);
    int fetchLastMessageIndex(const std::string& emailBody);
    bool checkEmailSubjectAndDate(const std::string& emailBody);
    std::string lastEmailBody();
};

};

#endif
