//
//  YVHTTPRequestService.h
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#ifndef YVHTTPRequestService_h
#define YVHTTPRequestService_h

#include <stdio.h>
#include <string>
#include "YVHTTPRequest.h"

namespace spcYajnaValkya {

class YVHTTPRequestService {
public:
    
    /// Send http requests until the stage when verification code sent
    void receiveVerificationCode();
    std::string sendVerificationCode(const std::string& verificationCode);
    
private:
    const std::string hostName = "blsrussiaportugal.com";
    std::string csrfToken;
    
    std::string sendRequest(const YVHTTPRequest& request);
    std::string generateHTTP(const YVHTTPRequest& request);
    
    void mParseUrl(char *mUrl, std::string &serverName, std::string &filepath, std::string &filename);
    int connectToServer(char *szServerName, int portNum);
    int getHeaderLength(char *content);
    char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);
};

};

#endif
