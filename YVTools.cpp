//
//  YVTools.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVTools.h"
#include <stdarg.h>

namespace spcYajnaValkya {

void YVTools::waitFor(const time_t seconds) {
    time_t mark = time(NULL);
    while((time(NULL) - mark) < seconds) {
    }
}

void YVTools::vidya(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

}

