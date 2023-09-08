//
//  YVTelegramBotAPI.hpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#ifndef YVTelegramBotAPI_h
#define YVTelegramBotAPI_h

#include <stdio.h>
#include <string>

#include "YVHTTPRequestService.h"

namespace spcYajnaValkya {

class YVTelegramBotAPI {
private:
    const std::string botID;
    YVHTTPRequestService httpService;
    
public:
    YVTelegramBotAPI(const std::string& botID);
    ~YVTelegramBotAPI();
    
    void sendMessage(const std::string& message);
};

};

#endif /* YVTelegramBotAPI_hpp */
