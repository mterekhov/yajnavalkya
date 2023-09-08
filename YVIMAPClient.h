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

    std::string sendRequest(const std::string& request, BIO *bio);
    int fetchLastMessageIndex(const std::string& emailBody);
    bool checkEmailSubjectAndDate(const std::string& emailBody);
    std::string lastEmailBody();
};

};

#endif
