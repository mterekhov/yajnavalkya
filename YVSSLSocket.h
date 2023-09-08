//
//  YVSSLSocket.h
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#ifndef YVSSLSocket_h
#define YVSSLSocket_h

#include <stdio.h>
#include <string>
#include <openssl/ssl.h>

namespace spcYajnaValkya {

class YVSSLSocket {
public:
    YVSSLSocket(const std::string& host, const int port);
    ~YVSSLSocket();
    std::string sendRequest(const std::string& request);

private:
    const std::string host;
    const int port;
    SSL *ssl;
    int openedSocket;

    int connectToServer(const std::string host, const int port);
    SSL *enableSSL(const int socket);
};

};


#endif /* YVSSLSocket_h */
