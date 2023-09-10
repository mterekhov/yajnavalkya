//
//  YVContentChecker.cpp
//  yajnavalkya
//
//  Created by cipher on 11.09.2023.
//

#include "YVContentChecker.h"

namespace spcYajnaValkya {

/// Check that the content of initial page is correct
/// - Parameter response: true in case everything is ok
bool YVContentChecker::checkStep_01(const std::string& response) {
    if (response.find("Семьи/Группы") == std::string::npos) {
        return false;
    }
    
    return true;
}

/// Check that verification code was send to email successfully
/// - Parameter response: true in case everything is ok
bool YVContentChecker::checkStep_02(const std::string& response) {
    if (response.find("pass") == std::string::npos) {
        return false;
    }
    
    return true;
}

/// Check that the content of agreement page contains agreement itself
/// - Parameter response: true in case everything is ok
bool YVContentChecker::checkStep_03(const std::string& response) {
    if (response.find("National Police Force") == std::string::npos) {
        return false;
    }
    
    return true;
}

/// Check that the agreement was successfully signed
/// - Parameter response: true in case everything is ok
bool YVContentChecker::checkStep_04(const std::string& response) {
    if (response.find("<script>document.location.href='appointment_family.php'</script>") == std::string::npos) {
        return false;
    }
    
    return true;
}

/// Check that the content of page with schedule calendar is correct
/// - Parameter response: true in case everything is ok
bool YVContentChecker::checkStep_05(const std::string& response) {
    if (response.find("<option value='6' selected>ORDINARY PASSPORT</option>") == std::string::npos) {
        return false;
    }
    
    return true;
}

}
