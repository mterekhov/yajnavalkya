//
//  YVHTTPRequestService.cpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#include "YVHTTPRequestService.h"
#include "YVSSLSocket.h"

namespace spcYajnaValkya {

YVHTTPRequestService::YVHTTPRequestService(const std::string& host) : hostName(host) {
    
}

YVHTTPRequestService::~YVHTTPRequestService() {
    
}

std::string YVHTTPRequestService::sendPOSTRequest(const std::string& link, const HTTPParsType& pars, const HTTPParsType& additionalHeaders) {
    return sendHTTPSRequest("POST", link, pars, additionalHeaders);
}

std::string YVHTTPRequestService::sendGETRequest(const std::string& link, const HTTPParsType& additionalHeaders) {
    return sendHTTPSRequest("GET", link, HTTPParsType(), additionalHeaders);
}

std::string YVHTTPRequestService::sendHTTPSRequest(const std::string& method,
                                                   const std::string& link,
                                                   const HTTPParsType& parsList,
                                                   const HTTPParsType& additionalHeaders) {
    YVSSLSocket socket(hostName, 443);
    std::string httpRequest = method + " ";
    httpRequest += link;
    httpRequest += " HTTP/1.1\r\n";
    httpRequest += "Host: ";
    httpRequest += hostName;
    if (!additionalHeaders.empty()) {
        httpRequest += "\r\n";
        httpRequest += generateHeaders(additionalHeaders);
    }

    if (!parsList.empty()) {
        std::string body = generateBody(parsList);
        char buffer[256] = {0};
        std::snprintf(buffer, 256, "\r\nContent-Length: %i", body.length());
        httpRequest += buffer;
        httpRequest += "\r\n\r\n";
        httpRequest += body;
    }
    else {
        httpRequest += "\r\n\r\n";
    }
    
    printf("REQUEST:\n%s\n", httpRequest.c_str());
    std::string response = socket.sendRequest(httpRequest);
    printf("RESPONSE:\n%s\n", response.c_str());
    
    return response;
}

std::string YVHTTPRequestService::generateHeaders(const HTTPParsType& additionalHeaders) {
    std::string headersString;

    for (HTTPParsType::const_iterator iter = additionalHeaders.begin(); iter != additionalHeaders.end(); iter++) {
        headersString += iter->key + ": " + iter->value + "\r\n";
    }
    headersString.pop_back();
    headersString.pop_back();

    return headersString;
}

std::string YVHTTPRequestService::generateBody(const HTTPParsType& parsList) {
    std::string newBody;

    for (HTTPParsType::const_iterator iter = parsList.begin(); iter != parsList.end(); iter++) {
        newBody += iter->key + "=" + iter->value + "&";
    }
    
    newBody.pop_back();
    
    return newBody;
}

};
