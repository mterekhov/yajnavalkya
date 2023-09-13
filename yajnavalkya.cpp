//
//  main.cpp
//  mail_sender
//
//  Created by cipher on 24.08.2023.
//

#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "YVScheduleService.h"

void daemonization(spcYajnaValkya::YVScheduleService& scheduleService) {
    
    scheduleService.launchSchedule();
//    struct pidfh *pfh = NULL;
//    pid_t otherpid = 0;
//
//    signal(SIGABRT, &control);
//    signal(SIGALRM, &control);
//    signal(SIGILL, &control);
//    signal(SIGFPE, &control);
//    signal(SIGHUP, &control);
//    signal(SIGINFO, &control);
//    signal(SIGINT, &control);
//
//    pfh = pidfile_open("/var/run/yajnavalkya.pid", 0600, &otherpid);
//    if (pfh == NULL) {
//        if (errno == EEXIST)
//        {
//            fprintf(stderr, "Daemon already running, pid: %d.", otherpid);
//            exit(EXIT_FAILURE);
//        }
//        fputs("Cannot open or create pidfile", stderr);
//    }
//
//    if (daemon(0, 0) == -1) {
//        fputs("daemon() fails.", stderr);
//        pidfile_remove(pfh);
//        exit(EXIT_FAILURE);
//    }
//
//    pidfile_write(pfh);
//
//    launchSchedule();
}

int main(int argc, const char * argv[]) {
    spcYajnaValkya::YVScheduleService scheduleService(120);
    
#ifdef __FreeBSD__
    printf("daemonization\n");
    daemonization(scheduleService);
#else
    printf("stright launch\n");
    scheduleService.launchSchedule();
#endif
    
    return 0;
}

//std::string recognizeCaptcha(const std::string& filename) {
//    tesseract::TessBaseAPI *tesseract = new tesseract::TessBaseAPI();
//    if (tesseract->Init(NULL, "eng")) {
//        printf("Tesseract failed to init");
//        return "";
//    }
//    Pix *image = pixRead(filename.c_str());
//    tesseract->SetImage(image);
//    std::string result = tesseract->GetUTF8Text();
//
//    tesseract->End();
//    pixDestroy(&image);
//
//    return result;
//}


