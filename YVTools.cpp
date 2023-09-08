//
//  YVTools.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVTools.h"

namespace spcYajnaValkya {

void YVTools::waitFor(const time_t seconds) {
    time_t mark = time(NULL);
    while((time(NULL) - mark) < seconds) {
        
    }
    
    return;
}

}

