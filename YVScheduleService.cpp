//
//  YVScheduleService.cpp
//  yajnavalkya
//
//  Created by cipher on 13.09.2023.
//

#include "YVScheduleService.h"
#include "YVTools.h"
#include "YVBLSRussiaPortugalAPI.h"
#include "YVIMAPClient.h"

#include <time.h>

namespace spcYajnaValkya {

YVScheduleService::YVScheduleService() : telegramBot("6356062369:AAHGg0paAAwIaWX8sC-4S1LQECjskaesHb0") {
    
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
    
    YVTools::vidyaInfo("\t\tSingle time slots\n");
    checkSingleSchedule();
//    YVTools::vidyaInfo("\t\tFamily time slots\n");
//    checkFamilySchedule();
    
    YVTools::vidyaInfo("Cycle finished\n");
}

void YVScheduleService::checkSingleSchedule() {
    YVBLSRussiaPortugalAPI apiService;
    YVTools::vidyaInfo("\t\trequesting verification code ...\n");
    if (!apiService.requestVerificationCode()) {
        return;
    };
    
    YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", 120);
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
    if (!apiService.termsOfUseAgree()) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tchecking for free slots ...\n");
    if (apiService.scheduleAppointment()) {
        YVTools::vidyaInfo("\t\tfree time slots found!\n");
        telegramBot.sendMessage("Free time slots found!\n");
    }
}

void YVScheduleService::checkFamilySchedule() {
    YVBLSRussiaPortugalAPI apiService;
    YVTools::vidyaInfo("\t\trequesting verification code ...\n");
    if (!apiService.requestVerificationCode()) {
        return;
    };
    
    YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", 120);
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
    if (!apiService.termsOfUseAgree()) {
        return;
    }
    
    YVTools::vidyaInfo("\t\tchecking for free slots ...\n");
    if (apiService.scheduleAppointment()) {
        YVTools::vidyaInfo("\t\tfree time slots found!\n");
        telegramBot.sendMessage("Free time slots found!\n");
    }
}

}
