//
//  YVTelegramBotAPI.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVTelegramBotAPI.h"

namespace spcYajnaValkya {

YVTelegramBotAPI::YVTelegramBotAPI(const std::string& botID) : httpService("api.telegram.org"), botID(botID) {
    
}

YVTelegramBotAPI::~YVTelegramBotAPI() {
    
}

void YVTelegramBotAPI::sendMessage(const std::string& message) {
    std::string link = "/bot" + botID + "/sendMessage";
    std::string response = httpService.sendGETRequest(link, {
        {"chat_id", "@adityaloka"},
        {"text", message}
                                                        },
                                                       {
        {"accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"}
                                                        });
}

}
