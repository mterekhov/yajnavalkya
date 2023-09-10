//
//  YVVerificationCode.cpp
//  yajnavalkya
//
//  Created by cipher on 28.08.2023.
//

#include "YVIMAPClient.h"
#include "YVTools.h"

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

YVIMAPClient::YVIMAPClient(const std::string& host, const int port, const std::string& login, const std::string& password, const time_t timeArea) : host(host), port(port), login(login), password(password), timeArea(timeArea) {
}

YVIMAPClient::~YVIMAPClient() {
    
}

std::string YVIMAPClient::fetchVerificationCode() {
    time_t mark = time(0);
    
    while ((time(NULL) - mark) < timeArea) {
        const std::string emailBody = lastEmailBody();
        if (checkEmailSubjectAndDate(emailBody)) {
            return fetchOTP(lastEmailBody());
        }
        else {
            YVTools::vidyaWarning("YVIMAPClient: email with verification code was not found\n");
        }
        YVTools::waitFor(refreshPeriod);
    }
    
    return "";
}

bool YVIMAPClient::checkEmailSubjectAndDate(const std::string& emailBody) {
    std::string timeDateString = YVTools::parse("Date: ",
                                                "\r\n",
                                                emailBody);
    struct tm emailTime = {0};
    strptime(timeDateString.c_str(), "%a, %d %b %Y %H:%M:%S +%z", &emailTime);

    std::string subject = YVTools::parse("From: ",
                                         "\r\n",
                                         emailBody);

    if (((time(0) - mktime(&emailTime)) < timeArea) && (subject == "BLS <info.portugal@blshelpline.com>")) {
        return true;
    }

    return false;
}

std::string YVIMAPClient::fetchOTP(const std::string emailBody) {
    return YVTools::parse("<td colspan=\"5\" style=\"text-align:left\" width=\"50%\">Verification code - ",
                          "<br/></td>",
                          emailBody);
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
    std::snprintf(buffer, BUFSIZ, "%s:%i", host.c_str(), port);
    BIO_set_conn_hostname(bio, buffer);
    BIO_do_connect(bio);
    
    memset(buffer, 0, BUFSIZ);
    BIO_read(bio, buffer, sizeof(buffer));
    
    std::string request = "tag LOGIN " + login + "@" + host + " " + password + "\r\n";
    std::string response = sendRequest(request, bio);
    
    request = "tag LIST \"\" \"*\"\r\n";
    response = sendRequest(request, bio);

    request = "tag select INBOX\r\n";
    response = sendRequest(request, bio);

    request = "tag STATUS INBOX (MESSAGES)\r\n";
    response = sendRequest(request, bio);

    int lastMessageIndex = fetchLastMessageIndex(response);
    if (lastMessageIndex < 0) {
        YVTools::vidyaWarning("YVIMAPClient: last message index was not found in IMAP response\n");
    }
    else {
        std::string mask = "tag FETCH %i (BODY[HEADER.FIELDS (FROM DATE)] BODY[TEXT])\r\n";
        memset(buffer, 0, BUFSIZ);
        std::snprintf(buffer, BUFSIZ, mask.c_str(), lastMessageIndex);
        response = sendRequest(buffer, bio);
    }
    
    BIO_free(bio);
    SSL_CTX_free(ctx);
    
    return response;
}

int YVIMAPClient::fetchLastMessageIndex(const std::string& emailBody) {
    std::string indexString = YVTools::parse("(MESSAGES ",
                                             ")",
                                             emailBody);
    if (indexString.empty()) {
        return -1;
    }
    
    return atoi(indexString.c_str());
}

std::string YVIMAPClient::sendRequest(const std::string& request, BIO *bio) {
    BIO_puts(bio, request.c_str());
    
    char buffer[BUFSIZ] = {0};
    BIO_read(bio, buffer, sizeof(buffer));
    
    return buffer;
}

}

