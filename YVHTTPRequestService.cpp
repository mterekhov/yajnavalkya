//
//  YVHTTPRequestService.cpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#include "YVHTTPRequestService.h"

#include <iomanip>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace spcYajnaValkya {

static std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

static std::string DecimalToHexadecimal(int dec) {
    if (dec < 1) return "0";

    int hex = dec;
    std::string hexStr;

    while (dec > 0)
    {
        hex = dec % 16;

        if (hex < 10)
            hexStr = hexStr.insert(0, std::string(1, (char)(hex + 48)));
        else
            hexStr = hexStr.insert(0, std::string(1, (char)(hex + 55)));

        dec /= 16;
    }

    return hexStr;
}

static std::string EncodeURL(std::string data) {
    std::string result;

    for (char c : data)
    {
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9'))
        {
            result += c;
        }
        else
        {
            result += '%';

            std::string s = DecimalToHexadecimal(c);

            if (s.length() < 2)
                s = s.insert(0, "0");

            result += s;
        }
    }

    return result;
}

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
    int socket = connectToServer(hostName);
    if (socket < 0) {
        return "";
    }
    SSL *ssl = enableSSL(socket);
    
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
    int err = SSL_write(ssl, httpRequest.c_str(), httpRequest.length());
    if (!err) {
        printf("error while sending");
    }
    
    char buffer[BUFSIZ + 1] = {0};
    std::string response = "";
    ssize_t bytes = 1;
    while (bytes > 0) {
        memset(buffer, 0, BUFSIZ);
        bytes = SSL_read(ssl, buffer, BUFSIZ);
        response += buffer;
    }
    
    //  cleanup
    SSL_shutdown(ssl);
    close(socket);
    SSL_CTX *ctx = SSL_get_SSL_CTX(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
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

int YVHTTPRequestService::connectToServer(const std::string host) {
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0) {
        printf("Error creating socket.\n");
        return -1;
    }
    
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    struct hostent *he;
    if ((he = gethostbyname(host.c_str())) == NULL) {
        printf(">>> host address is not defined\n");
        return -1;
    }
    socketAddress.sin_addr.s_addr = *((unsigned long*)he->h_addr);
    socketAddress.sin_port = htons(443);
    if (connect(newSocket, (struct sockaddr *)&socketAddress, sizeof(socketAddress))) {
        printf(">> Error connecting to server.\n");
        return -1;
    }
    
    return newSocket;
}

SSL *YVHTTPRequestService::enableSSL(const int socket) {
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    SSL* newSSL = SSL_new(ctx);
    if (!newSSL) {
        printf(">>> Error creating SSL.\n");
        return NULL;
    }
    
    SSL_set_fd(newSSL, socket);
    int err = SSL_connect(newSSL);
    if (err <= 0) {
        printf(">>> Error creating SSL connection.  err=%x\n", err);
        return NULL;
    }
    
    return newSSL;
}

};
