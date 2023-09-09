//
//  main.cpp
//  mail_sender
//
//  Created by cipher on 24.08.2023.
//

#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <time.h>

#include "YVIMAPClient.h"
#include "YVBLSRussiaPortugalAPI.h"
#include "YVTelegramBotAPI.h"

std::string recognizeCaptcha(const std::string& filename) {
	tesseract::TessBaseAPI *tesseract = new tesseract::TessBaseAPI();
	if (tesseract->Init(NULL, "eng")) {
		printf("Tesseract failed to init");
		return "";
	}
	Pix *image = pixRead(filename.c_str());
	tesseract->SetImage(image);
	std::string result = tesseract->GetUTF8Text();

	tesseract->End();
	pixDestroy(&image);

	return result;
}

void checkTimeSlots() {
    spcYajnaValkya::YVTelegramBotAPI telegramBot("6356062369:AAHGg0paAAwIaWX8sC-4S1LQECjskaesHb0");
    spcYajnaValkya::YVBLSRussiaPortugalAPI apiService;
    printf("YajnaValkya: requesting verification code ... ");
    apiService.requestVerificationCode();
    printf("DONE\n");
    
    spcYajnaValkya::YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", 120);
    printf("YajnaValkya: receiving verification code from email ... ");
    std::string verificationCode = imapClient.fetchVerificationCode();
    if (verificationCode.empty()) {
        printf("YajnaValkya: verification code was not found in email");
        return;
    }
    printf("DONE\n");
    
    printf("YajnaValkya: requesting appointment ... ");
    apiService.requestAppointment(verificationCode);
    printf("DONE\n");
    
    printf("YajnaValkya: signing agreement ... ");
    apiService.termsOfUseAgree();
    printf("DONE\n");
    
    printf("YajnaValkya: checking for free slots ... ");
    if (apiService.scheduleAppointment()) {
        printf("YajnaValkya: free time slots found! ");
        telegramBot.sendMessage("Free time slots found!\n");
        return;
    }
    
    printf("YajnaValkya: no free time slots found\n");
}

int main(int argc, const char * argv[]) {
    time_t waitingPeriod = 3 * 60;   //  5 minutes
    time_t longWaitingPeriod = 10 * 60;   //  5 minutes
    time_t lastMark = 0;
    bool introMessageSent = false;
    bool longIntroMessageSent = false;
    spcYajnaValkya::YVTelegramBotAPI telegramBot("6356062369:AAHGg0paAAwIaWX8sC-4S1LQECjskaesHb0");
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
                checkTimeSlots();
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
                checkTimeSlots();
                lastMark = time(0);
            }
        }
    }
    telegramBot.sendMessage("Monitoring is finished");

    return 0;
}
