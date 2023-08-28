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

void YVHTTPRequestService::receiveVerificationCode() {
    const int bufLen = 1024;
    char *szUrl = "https://blsrussiaportugal.com/russian/appointment_family.php"; //    "http://stackoverflow.com";
    long fileSize;
    char *memBuffer = NULL;
    char *headerBuffer = NULL;
    FILE *fp;

    memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);
    printf("returned from readUrl\n");
    printf("data returned:\n%s", memBuffer);
    if (fileSize != 0) {
        printf("Got some data\n");
        fp = fopen("downloaded.file", "wb");
        fwrite(memBuffer, 1, fileSize, fp);
        fclose(fp);
         delete(memBuffer);
        delete(headerBuffer);
    }

//    sendVerificationCode("");
}
    
std::string YVHTTPRequestService::sendVerificationCode(const std::string& verificationCode) {
    YVHTTPRequest startRequest = {"/russian/appointment_family.php", "", GET};
    sendRequest(startRequest);
    return "";
}

std::string YVHTTPRequestService::sendRequest(const YVHTTPRequest& request) {
    OPENSSL_malloc_init();
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    BIO *bio = BIO_new_ssl_connect(ctx);
    SSL *ssl;
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    
    BIO_set_conn_hostname(bio, (hostName + ":80").c_str());
    BIO_do_connect(bio);
    char tmp[2048] = {0};
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    
    BIO_puts(bio, generateHTTP(request).c_str());
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    printf(tmp);
    
    return "";
}

std::string YVHTTPRequestService::generateHTTP(const YVHTTPRequest& request) {
    std::string finalHTTP;

    //  Request type
    switch (request.type) {
        case GET:
            finalHTTP += "GET";
            break;
        case POST:
            finalHTTP += "POST";
            break;
    }
    finalHTTP += request.link + " HTTP/1.1" + "\r\n";
    
    finalHTTP += "HOST: " + hostName + "\r\n";
    
    finalHTTP += "Connection: keep-alive\r\n";
    
    return finalHTTP;

//    GET /russian/book_appointment.php HTTP/1.1
//    Host: blsrussiaportugal.com
//    Sec-Fetch-Site: same-origin
//    Accept-Encoding: gzip, deflate, br
//    Cookie: _ga=GA1.2.577806393.1692845969; _ga_TMNRH3PRZJ=GS1.2.1692845969.1.1.1692846025.0.0.0; _gid=GA1.2.82681429.1692845969; PHPSESSID=6nn8ka1j4qbin3ntn5f9ujtdh0
//    Connection: keep-alive
//    Sec-Fetch-Mode: navigate
//    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//    User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.5.2 Safari/605.1.15
//    Referer: https://blsrussiaportugal.com/russian/appointment_family.php
//    Sec-Fetch-Dest: document
//    Accept-Language: en-GB,en;q=0.9


}

void YVHTTPRequestService::mParseUrl(char *mUrl, std::string &serverName, std::string &filepath, std::string &filename) {
    std::string::size_type n;
    std::string url = mUrl;

    if (url.substr(0,7) == "http://")
        url.erase(0,7);

    if (url.substr(0,8) == "https://")
        url.erase(0,8);

    n = url.find('/');
    if (n != std::string::npos) {
        serverName = url.substr(0,n);
        filepath = url.substr(n);
        n = filepath.rfind('/');
        filename = filepath.substr(n+1);
    }
    else {
        serverName = url;
        filepath = "/";
        filename = "";
    }
}

int YVHTTPRequestService::connectToServer(char *szServerName, int portNum) {
    struct hostent *hp;
    unsigned int addr;
    struct sockaddr_in server;

    int newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (newSocket == -1) {
        return NULL;
    }

    if (inet_addr(szServerName) == INADDR_NONE) {
        hp = gethostbyname(szServerName);
    }
    else {
        addr = inet_addr(szServerName);
        hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
    }

    if (hp == NULL) {
        close(newSocket);
        return NULL;
    }

    server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);
    if (connect(newSocket,(struct sockaddr*)&server, sizeof(server))) {
        close(newSocket);
        return NULL;
    }
    
    return newSocket;
}

int YVHTTPRequestService::getHeaderLength(char *content) {
    const char *srchStr1 = "\r\n\r\n";
    const char *srchStr2 = "\n\r\n\r";
    int offset = -1;

    char *findPos = strstr(content, srchStr1);
    if (findPos != NULL) {
        offset = findPos - content;
        offset += strlen(srchStr1);
    }
    else {
        findPos = strstr(content, srchStr2);
        if (findPos != NULL) {
            offset = findPos - content;
            offset += strlen(srchStr2);
        }
    }
    
    return offset;
}

char *YVHTTPRequestService::readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut) {
    const int bufSize = 512;
    char readBuffer[bufSize];
    char sendBuffer[bufSize];
    char tmpBuffer[bufSize];
    char *tmpResult = NULL;
    char *result;
    std::string server;
    std::string filepath;
    std::string filename;
    long totalBytesRead;
    long thisReadSize;
    long headerLen;

    mParseUrl(szUrl, server, filepath, filename);

    ///////////// step 1, connect //////////////////////
    int conn = connectToServer((char*)server.c_str(), 80);

    ///////////// step 2, send GET request /////////////
    std::string finalHTTP = "GET";
    finalHTTP += filepath + " HTTP/1.1" + "\r\n";
    finalHTTP += "Host: " + hostName + "\r\n";
    send(conn, finalHTTP.c_str(), finalHTTP.length(), 0);

    printf("Buffer being sent:\n%s", finalHTTP.c_str());

    ///////////// step 3 - get received bytes ////////////////
    // Receive until the peer closes the connection
    totalBytesRead = 0;
    while(1) {
        memset(readBuffer, 0, bufSize);
        thisReadSize = recv(conn, readBuffer, bufSize, 0);

        if ( thisReadSize <= 0 ) {
            break;
        }

        tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

        memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
        totalBytesRead += thisReadSize;
    }

    std::string completeResponse = tmpResult;
    headerLen = getHeaderLength(tmpResult);
    long contenLen = totalBytesRead - headerLen;
    result = new char[contenLen + 1];
    memcpy(result, tmpResult + headerLen, contenLen);
    result[contenLen] = 0x0;

    char *myTmp = new char[headerLen + 1];
    strncpy(myTmp, tmpResult, headerLen);
    myTmp[headerLen] = NULL;
    delete(tmpResult);
    *headerOut = myTmp;

    bytesReturnedOut = contenLen;
    close(conn);
    return(result);
}

};
