//
//  YVTools.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVTools.h"
#include <time.h>
#include <string>

namespace spcYajnaValkya {

void YVTools::waitFor(const time_t seconds) {
    time_t mark = time(NULL);
    while((time(NULL) - mark) < seconds) {
    }
}

void YVTools::vidyaInfo(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vidya(YVTOOLS_VIDYA_INFO, format, args);
    va_end(args);
}

void YVTools::vidyaError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vidya(YVTOOLS_VIDYA_ERROR, format, args);
    va_end(args);
}

void YVTools::vidyaWarning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vidya(YVTOOLS_VIDYA_WARNING, format, args);
    va_end(args);
}

void YVTools::vidya(const YVToolsVidyaType type, const char* format, va_list args) {
#ifdef NDEBUG
    return;
#endif
    time_t rawtime;
    time(&rawtime);
    char timeBuffer[BUFSIZ] = {0};
    char *timeFormat = "%d-%m-%Y %H:%M:%S";
    strftime(timeBuffer, BUFSIZ, timeFormat, localtime(&rawtime));

    std::string newFormat = timeBuffer;
    switch (type) {
        case YVTOOLS_VIDYA_INFO:
            newFormat += " YajnaValkya: ";
            break;
        case YVTOOLS_VIDYA_WARNING:
            newFormat += " YajnaValkya-WARNING: ";
            break;
        case YVTOOLS_VIDYA_ERROR:
            newFormat += " YajnaValkya-ERROR: ";
            break;

        default:
            break;
    }
    newFormat += format;

    vprintf(newFormat.c_str(), args);
}

}

