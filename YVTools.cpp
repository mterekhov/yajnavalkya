//
//  YVTools.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVTools.h"
#include <stdarg.h>
#include <time.h>
#include <string>

namespace spcYajnaValkya {

void YVTools::waitFor(const time_t seconds) {
    time_t mark = time(NULL);
    while((time(NULL) - mark) < seconds) {
    }
}

void YVTools::vidya(const char* format, ...) {
#ifdef NDEBUG
    return;
#endif
    time_t rawtime;
    time(&rawtime);
    char timeBuffer[BUFSIZ] = {0};
    char *timeFormat = "%d-%m-%Y %H:%M:%S";
    strftime(timeBuffer, BUFSIZ, timeFormat, localtime(&rawtime));

    std::string newFormat = timeBuffer;
    newFormat += " YajnaValkya: ";
    newFormat += format;

    va_list args;
    va_start(args, newFormat.c_str());
    
    vprintf(newFormat.c_str(), args);
    va_end(args);
}

}

