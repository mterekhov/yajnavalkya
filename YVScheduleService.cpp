//
//  YVScheduleService.cpp
//  yajnavalkya
//
//  Created by cipher on 13.09.2023.
//

#include "YVScheduleService.h"
#include "YVTools.h"
#include "YVIMAPClient.h"

#include <time.h>

namespace spcYajnaValkya {

YVScheduleService::YVScheduleService(const int timeArea) : telegramBot("6356062369:AAHGg0paAAwIaWX8sC-4S1LQECjskaesHb0"), timeArea(timeArea) {
    
}

YVScheduleService::~YVScheduleService() {
    
}

void YVScheduleService::launchSchedule() {
    int cycle = 1;
    time_t waitingPeriod = 3 * 60;   //  5 minutes
    time_t longWaitingPeriod = 10 * 60;   //  5 minutes
    time_t lastMark = 0;
    bool introMessageSent = false;
    bool longIntroMessageSent = false;
    YVTools::vidyaInfo("Launching ...\n");
    telegramBot.sendMessage("Start to monitor the schedule");
    while(true) {
        time_t rawtime;
        time(&rawtime);
        struct tm *timeinfo = localtime(&rawtime);
        if ((timeinfo->tm_hour >= 0) && (timeinfo->tm_hour < 3)) {
            if (!introMessageSent) {
                longIntroMessageSent = false;
                introMessageSent = true;
                telegramBot.sendMessage("begining to check time slots frequently");
            }
            if ((time(0) - lastMark) > waitingPeriod) {
                checkTimeSlots(cycle);
                lastMark = time(0);
            }
        }
        else {
            if ((time(0) - lastMark) > longWaitingPeriod) {
                if (!longIntroMessageSent) {
                    longIntroMessageSent = true;
                    introMessageSent = false;
                    telegramBot.sendMessage("switched to sleepy check every 10 minutes");
                }
                checkTimeSlots(cycle);
                lastMark = time(0);
            }
        }
    }
    telegramBot.sendMessage("Monitoring is finished");
}

void YVScheduleService::checkTimeSlots(int& cycle) {
    YVTools::vidyaInfo("Starting new cycle %04i\n", cycle++);
    
    YVTools::vidyaInfo("\t\tSingle time slots check\n");
    checkSingleSchedule();
    
    YVTools::waitFor(timeArea);
    
    YVTools::vidyaInfo("\t\tFamily time slots check\n");
    checkFamilySchedule();
    
    YVTools::vidyaInfo("Cycle finished\n");
}

void YVScheduleService::checkSingleSchedule() {
    YVBLSRussiaPortugalAPI apiService;
    YVTools::vidyaInfo("\t\trequesting verification code ...\n");
    if (!apiService.requestVerificationCode()) {
        return;
    };
    
    YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", timeArea);
    YVTools::vidyaInfo("\t\treceiving verification code from email ...\n");
    std::string verificationCode = imapClient.fetchVerificationCode();
    if (verificationCode.empty()) {
        YVTools::vidyaInfo("\t\tverification code was not found in email\n");
        return;
    }
    
    YVTools::vidyaInfo("\t\trequesting appointment ...\n");
    if (!apiService.requestAppointment(SCHEDULE_REQUEST_SINGLE, verificationCode)) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tsigning agreement ...\n");
    if (!apiService.termsOfUseAgree(SCHEDULE_REQUEST_SINGLE)) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tchecking for free slots ...\n");
    if (apiService.scheduleAppointment(SCHEDULE_REQUEST_SINGLE)) {
        timeSlotsFoundNotification(SCHEDULE_REQUEST_SINGLE);
    }
}

void YVScheduleService::checkFamilySchedule() {
    YVBLSRussiaPortugalAPI apiService;
    YVTools::vidyaInfo("\t\trequesting verification code ...\n");
    if (!apiService.requestVerificationCode()) {
        return;
    };
    
    YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", timeArea);
    YVTools::vidyaInfo("\t\treceiving verification code from email ...\n");
    std::string verificationCode = imapClient.fetchVerificationCode();
    if (verificationCode.empty()) {
        YVTools::vidyaInfo("\t\tverification code was not found in email\n");
        return;
    }
    
    YVTools::vidyaInfo("\t\trequesting appointment ...\n");
    if (!apiService.requestAppointment(SCHEDULE_REQUEST_FAMILY, verificationCode)) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tsigning agreement ...\n");
    if (!apiService.termsOfUseAgree(SCHEDULE_REQUEST_FAMILY)) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tchecking for free slots ...\n");
    if (apiService.scheduleAppointment(SCHEDULE_REQUEST_FAMILY)) {
        timeSlotsFoundNotification(SCHEDULE_REQUEST_FAMILY);
    }
}

void YVScheduleService::timeSlotsFoundNotification(const EScheduleRequestType type) {
    YVTools::vidyaInfo("\t\tsingle free time slots found!\n");
    telegramBot.sendMessage("ACHTUNG!!!");
    telegramBot.sendMessage("ACHTUNG!!!");
    telegramBot.sendMessage("ACHTUNG!!!");
    switch (type) {
        case SCHEDULE_REQUEST_FAMILY:
            telegramBot.sendMessage("FAMILY free time slots found!");
            break;
        case SCHEDULE_REQUEST_SINGLE:
            telegramBot.sendMessage("SINGLE free time slots found!");
            break;
            
    }
    telegramBot.sendMessage("ACHTUNG!!!");
    telegramBot.sendMessage("ACHTUNG!!!");
    telegramBot.sendMessage("ACHTUNG!!!");
}

}
