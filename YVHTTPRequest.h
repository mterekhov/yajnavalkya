//
//  YVHTTPRequest.hpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#ifndef YVHTTPRequest_h
#define YVHTTPRequest_h

#include <stdio.h>
#include <string>

enum HTTRequestType {
    GET,
    POST
};

struct YVHTTPRequest {
    std::string link;
    std::string pars;
    HTTRequestType type;
};

#endif /* YVHTTPRequest_hpp */
