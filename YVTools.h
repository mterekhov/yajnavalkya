//
//  YVTools.h
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#ifndef YVTools_h
#define YVTools_h

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string>

namespace spcYajnaValkya {

enum YVToolsVidyaType {
    YVTOOLS_VIDYA_INFO,
    YVTOOLS_VIDYA_WARNING,
    YVTOOLS_VIDYA_ERROR
};

class YVTools {
public:
    static std::string parse(const std::string& startMark, const std::string& endMark, const std::string& textToParse);
    static void waitFor(const time_t seconds);
    static void vidyaInfo(const char* format, ...);
    static void vidyaError(const char* format, ...);
    static void vidyaWarning(const char* format, ...);

private:
    static void vidya(const YVToolsVidyaType type, const char* format, va_list args);
};

};

#endif /* YVTools_h */
