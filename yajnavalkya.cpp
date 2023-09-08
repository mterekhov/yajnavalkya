//
//  main.cpp
//  mail_sender
//
//  Created by cipher on 24.08.2023.
//

#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

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

int main(int argc, const char * argv[]) {
    spcYajnaValkya::YVBLSRussiaPortugalAPI apiService;
    printf("YajnaValkya: requesting verification code ... ");
    apiService.requestVerificationCode();
    printf("DONE\n");
    
    spcYajnaValkya::YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", 120);
    printf("YajnaValkya: receiving verification code from email ... ");
    std::string verificationCode = imapClient.fetchVerificationCode();
    if (verificationCode.empty()) {
        printf("YajnaValkya: verification code was not found in email");
        return 0;
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
        spcYajnaValkya::YVTelegramBotAPI telegramBot("6356062369:AAHGg0paAAwIaWX8sC-4S1LQECjskaesHb0");
        printf("YajnaValkya: free time slots found! ");
        telegramBot.sendMessage("Free time slots found!\n");
        return 0;
    }
    
    printf("YajnaValkya: no free time slots found\n");

    return 0;
}
