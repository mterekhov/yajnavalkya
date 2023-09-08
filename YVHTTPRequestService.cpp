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

std::string YVHTTPRequestService::sendPOSTRequest(const std::string& link, const HTTPParsType& pars, const HTTPHeadersType& additionalHeaders) {
    return sendHTTPSRequest("POST", link, pars, additionalHeaders);
}

std::string YVHTTPRequestService::sendGETRequest(const std::string& link, const HTTPParsType& pars, const HTTPHeadersType& additionalHeaders) {
    return sendHTTPSRequest("GET", link, pars, additionalHeaders);
}

std::string YVHTTPRequestService::sendHTTPSRequest(const std::string& method,
                                                   const std::string& link,
                                                   const HTTPParsType& parsList,
                                                   const HTTPHeadersType& additionalHeaders) {
    YVSSLSocket socket(hostName, 443);
    std::string httpRequest = method + " ";
    if (!parsList.empty() && (method == "GET")) {
        httpRequest += link + generateGETParsString(parsList);
    }
    else {
        httpRequest += link;
    }
    httpRequest += " HTTP/1.1\r\n";
    httpRequest += "Host: ";
    httpRequest += hostName;
    if (!additionalHeaders.empty()) {
        httpRequest += "\r\n";
        httpRequest += generateHeaders(additionalHeaders);
    }

    if (!parsList.empty() && (method == "POST")) {
        std::string body = generatePOSTBody(parsList, additionalHeaders);
        char buffer[BUFSIZ] = {0};
        std::snprintf(buffer, BUFSIZ, "\r\nContent-Length: %i", body.length());
        httpRequest += buffer;
        httpRequest += "\r\n\r\n";
        httpRequest += body;
    }
    else {
        httpRequest += "\r\n\r\n";
    }
    
    return socket.sendRequest(httpRequest);
}

std::string YVHTTPRequestService::generateHeaders(const HTTPHeadersType& additionalHeaders) {
    std::string headersString;

    for (HTTPParsType::const_iterator iter = additionalHeaders.begin(); iter != additionalHeaders.end(); iter++) {
        headersString += iter->key + ": " + iter->value + "\r\n";
    }
    headersString.pop_back();
    headersString.pop_back();

    return headersString;
}

std::string YVHTTPRequestService::generatePOSTBody(const HTTPParsType& parsList, const HTTPHeadersType& additionalHeaders) {
    std::string newBody;

    bool generateJSONBody = false;
    for (HTTPParsType::const_iterator iter = additionalHeaders.begin(); iter != additionalHeaders.end(); iter++) {
        if ((iter->key == "content-type" ) && (iter->value == "application/json")) {
            generateJSONBody = true;
            break;
        }
    }

    if (generateJSONBody) {
        newBody = "{";
        for (HTTPParsType::const_iterator iter = parsList.begin(); iter != parsList.end(); iter++) {
            newBody += "\"" + iter->key + "\":\"" + iter->value + "\",";
        }
        newBody.pop_back();
        newBody += "}";
    }
    else {
        for (HTTPParsType::const_iterator iter = parsList.begin(); iter != parsList.end(); iter++) {
            newBody += iter->key + "=" + iter->value + "&";
        }
        newBody.pop_back();
    }
        
    return newBody;
}

std::string YVHTTPRequestService::generateGETParsString(const HTTPParsType& parsList) {
    std::string parsString = "?";

    for (HTTPParsType::const_iterator iter = parsList.begin(); iter != parsList.end(); iter++) {
        parsString += iter->key + "=" + iter->value + "&";
    }
    
    parsString.pop_back();
    
    return parsString;
}

};
