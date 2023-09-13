//
//  YVScheduleService.h
//  yajnavalkya
//
//  Created by cipher on 13.09.2023.
//

#ifndef YVScheduleService_hpp
#define YVScheduleService_hpp

#include <stdio.h>

#include "YVTelegramBotAPI.h"

namespace spcYajnaValkya {

class YVScheduleService {
public:
    YVScheduleService();
    ~YVScheduleService();
    
    void launchSchedule();

private:
    YVTelegramBotAPI telegramBot;

    void checkTimeSlots(int& cycle);
    void checkSingleSchedule();
    void checkFamilySchedule();
};

};

#endif
