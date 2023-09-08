//
//  YVVerificationCode.cpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#include "YVIMAPClient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace spcYajnaValkya {

YVIMAPClient::YVIMAPClient(const std::string& host, const int port, const std::string& login, const std::string& password) : host(host), port(port), login(login), password(password) {
}

YVIMAPClient::~YVIMAPClient() {
    
}

std::string YVIMAPClient::fetchVerificationCode() {
    const std::string emailBody = lastEmailBody();
    printf("EMAIL BODY:\n%s\n", emailBody.c_str());
    std::string startMark = "<td colspan=\"5\" style=\"text-align:left\" width=\"50%\">Verification code - ";
    std::string endMark = "<br/></td>";
    auto startPosition = emailBody.find(startMark);
    
    auto markStartPosition = emailBody.find(startMark);
    auto markEndPosition = emailBody.find(endMark, markStartPosition);
    std::string code = emailBody.substr(markStartPosition + startMark.length(), markEndPosition - markStartPosition - startMark.length());
    
    return code;
}

int YVIMAPClient::fetchLastMessageIndex(const std::string& emailBody) {
    std::string indexString;
    std::string startMark = "(MESSAGES ";
    auto markStartPosition = emailBody.find(startMark);
    auto markEndPosition = emailBody.find(")", markStartPosition);
    for (int i = 0; i < markEndPosition - markStartPosition - startMark.length(); i++) {
        indexString.append(emailBody.substr(markStartPosition + i + startMark.length(), 1));
    }
    
    return atoi(indexString.c_str());
}

bool YVIMAPClient::checkEmailSubjectAndDate(const std::string& emailBody) {
    return true;
}

std::string YVIMAPClient::lastEmailBody() {
    OPENSSL_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
    SSL_library_init(); // Initialize OpenSSL's SSL libraries
    SSL_load_error_strings(); // Load SSL error strings
    ERR_load_BIO_strings(); // Load BIO error strings
    OpenSSL_add_all_algorithms(); // Load all available encryption algorithms
    
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    BIO *bio = BIO_new_ssl_connect(ctx);
    SSL *ssl;
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    
    char buffer[BUFSIZ] = {0};
    std::snprintf(buffer, BUFSIZ, ":%i", port);
    std::string hostConnection = host + buffer;
    BIO_set_conn_hostname(bio, hostConnection.c_str());
    BIO_do_connect(bio);
    
    memset(buffer, 0, BUFSIZ);
    BIO_read(bio, buffer, sizeof(buffer));
    
    std::string request = "tag LOGIN " + login + "@" + host + " " + password + "\r\n";
    std::string response = sendRequest(request, bio);
    printf("RESPONSE:\n%s\n", response.c_str());
    
    request = "tag LIST \"\" \"*\"\r\n";
    response = sendRequest(request, bio);
    printf("RESPONSE:\n%s\n", response.c_str());

    request = "tag select INBOX\r\n";
    response = sendRequest(request, bio);
    printf("RESPONSE:\n%s\n", response.c_str());

    request = "tag STATUS INBOX (MESSAGES)\r\n";
    response = sendRequest(request, bio);
    printf("RESPONSE:\n%s\n", response.c_str());

    int lasMessageIndex = fetchLastMessageIndex(response);
    
    std::string mask = "tag FETCH %i (BODY[HEADER.FIELDS (FROM DATE)] BODY[TEXT])\r\n";
    memset(buffer, 0, BUFSIZ);
    std::snprintf(buffer, BUFSIZ, mask.c_str(), lasMessageIndex);
    response = sendRequest(buffer, bio);
    printf("RESPONSE:\n%s\n", response.c_str());

    BIO_free(bio);
    SSL_CTX_free(ctx);
    
    return response;
}

std::string YVIMAPClient::sendRequest(const std::string& request, BIO *bio) {
    BIO_puts(bio, request.c_str());
    
    char buffer[BUFSIZ] = {0};
    BIO_read(bio, buffer, sizeof(buffer));
    
    return buffer;
}

}

