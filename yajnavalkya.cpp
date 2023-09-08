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
    apiService.requestVerificationCode();
    
    spcYajnaValkya::YVIMAPClient imapClient("disroot.org", 993, "yajnavalkya", "cf1f3QUNc", 120);
    std::string verificationCode = imapClient.fetchVerificationCode();
    if (verificationCode.empty()) {
        printf("Yajnavalkya: verification code was not found in email");
        return 0;
    }
    
    apiService.requestAppointment(verificationCode);
    apiService.termsOfUseAgree();
    apiService.scheduleAppointment();
//
//    printf("text recognized <%s>\n", recognizeCaptcha("/Users/cipher/Desktop/captcha/captcha.jpeg").c_str());
    return 0;
}
