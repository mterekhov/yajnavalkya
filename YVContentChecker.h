//
//  YVContentChecker.hpp
//  yajnavalkya
//
//  Created by cipher on 11.09.2023.
//

#ifndef YVContentChecker_h
#define YVContentChecker_h

#include <stdio.h>
#include <string>

namespace spcYajnaValkya {

class YVContentChecker {
public:
    bool checkStep_01(const std::string& response);
    bool checkStep_02(const std::string& response);
    bool checkStep_03(const std::string& response);
    bool checkStep_04(const std::string& response);
    bool checkStep_04_single(const std::string& response);
    bool checkStep_05(const std::string& response);
    bool checkStep_05_single(const std::string& response);
};

};

#endif /* YVContentChecker_hpp */
