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
#include "YVBLSRussiaPortugalAPI.h"

namespace spcYajnaValkya {

class YVScheduleService {
public:
    YVScheduleService(const int timeArea);
    ~YVScheduleService();
    
    void launchSchedule();

private:
    //  it will wait for this amount of seconds between single and family check
    const int timeArea;
    YVTelegramBotAPI telegramBot;

    void checkTimeSlots(int& cycle);
    void checkSingleSchedule();
    void checkFamilySchedule();
    void timeSlotsFoundNotification(const EScheduleRequestType type);
};

};

#endif
