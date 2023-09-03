//
//  YVHTTPRequestService.cpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#include "YVHTTPRequestService.h"

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

YVHTTPRequestService::YVHTTPRequestService(const std::string& host) : hostName(host) {
    
}

YVHTTPRequestService::~YVHTTPRequestService() {
    
}

std::string YVHTTPRequestService::sendPOSTRequest(const std::string& link, const HTTPParsType& pars) {
    return sendHTTPSRequest("POST", link, pars);
}

std::string YVHTTPRequestService::sendGETRequest(const std::string& link) {
    return sendHTTPSRequest("GET", link, HTTPParsType());
}

std::string YVHTTPRequestService::sendHTTPSRequest(const std::string& method,
                                                   const std::string& link,
                                                   const HTTPParsType& parsList) {
    int socket = connectToServer(hostName);
    if (socket < 0) {
        return "";
    }
    SSL *ssl = enableSSL(socket);
    
    std::string httpRequest = method + " ";
    httpRequest += link;
    httpRequest += " HTTP/1.1\r\nHost: ";
    httpRequest += hostName;
    httpRequest += "\r\n\r\n";
    if (!parsList.empty()) {
        httpRequest += generateBody(parsList);
    }
    
    int err = SSL_write(ssl, httpRequest.c_str(), httpRequest.length());
    
    char buffer[BUFSIZ] = {0};
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
    printf("socket closed\n");
    SSL_CTX *ctx = SSL_get_SSL_CTX(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    return response;
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
    printf(">> SSL connection using %s\n", SSL_get_cipher(newSSL));
    
    return newSSL;
}

};
