//
//  YVIMAPClient.cpp
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

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace spcYajnaValkya {

YVIMAPClient::YVIMAPClient(const std::string& host, const int port, const std::string& login, const std::string& password) : host(host), port(port), login(login), password(password) {
    
}

YVIMAPClient::~YVIMAPClient() {
    
}

std::string YVIMAPClient::fetchVerificationCode(const time_t nearTime) {
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
    std::string startMark = "(UNSEEN ";
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
    
    //  connect
    char tmp[BUFSIZ] = {0};
    std::snprintf(tmp, BUFSIZ, "%s:%i", host.c_str(), port);
    BIO_set_conn_hostname(bio, tmp);
    BIO_do_connect(bio);
    
    memset(tmp, 0, BUFSIZ);
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    printf("YajnaValkya::YVIMAPClient: RESPONSE:\n%s\n", tmp);
    printf("//====================================================\n");

    //                   tag LOGIN yajnavalkya@disroot.org cf2f4QUNc
    //    BIO_puts(bio, "tag LOGIN yajnavalkya@disroot.org cf1f3QUNc\r\n");
    std::string request = "tag LOGIN " + login + "@" + host + " " + password + "\r\n";
    printf("YajnaValkya::YVIMAPClient: REQUEST:\n%s\n", request.c_str());
    BIO_puts(bio, request.c_str());
    memset(tmp, 0, BUFSIZ);
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    printf("YajnaValkya::YVIMAPClient: RESPONSE:\n%s\n", tmp);
    printf("//====================================================\n");

    BIO_puts(bio, "tag LIST \"\" \"*\"\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    
    BIO_puts(bio, "tag select INBOX\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    
    BIO_puts(bio, "tag STATUS INBOX (UNSEEN)\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    printf("Answer: <%s>\n", tmp);
    
    int lasMessageIndex = fetchLastMessageIndex(tmp);
    
    std::string mask = "tag FETCH %i (BODY[HEADER.FIELDS (FROM DATE)] BODY[TEXT])\r\n";
    char buffer[256] = {0};
    std::snprintf(buffer, 256, mask.c_str(), lasMessageIndex);
    BIO_puts(bio, buffer);
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    printf("Answer: <%s>\n", tmp);
    
    BIO_free(bio);
    SSL_CTX_free(ctx);
    
    return tmp;
}


}
