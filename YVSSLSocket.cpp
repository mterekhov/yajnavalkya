//
//  YVSSLSocket.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include "YVSSLSocket.h"

#include <openssl/err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace spcYajnaValkya {

YVSSLSocket::YVSSLSocket(const std::string& host, const int port) : host(host), port(port) {
    openedSocket = connectToServer(host, port);
    if (openedSocket < 0) {
        return;
    }
    ssl = enableSSL(openedSocket);
}

YVSSLSocket::~YVSSLSocket() {
    SSL_shutdown(ssl);
    close(openedSocket);
    SSL_CTX *ctx = SSL_get_SSL_CTX(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
}

std::string YVSSLSocket::sendRequest(const std::string& message) {
    int err = SSL_write(ssl, message.c_str(), message.length());
    if (!err) {
        printf("YajnaValkya::YVSSLSocket: error while sending");
    }
    
    char buffer[BUFSIZ + 1] = {0};
    std::string response = "";
    ssize_t bytes = 1;
    while (bytes > 0) {
        memset(buffer, 0, BUFSIZ);
        bytes = SSL_read(ssl, buffer, BUFSIZ);
        response += buffer;
    }
    
    return response;
}

int YVSSLSocket::connectToServer(const std::string host, const int port) {
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0) {
        printf("YajnaValkya::YVSSLSocket: error creating socket.\n");
        return -1;
    }
    
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    struct hostent *he;
    if ((he = gethostbyname(host.c_str())) == NULL) {
        printf("YajnaValkya::YVSSLSocket: host address is not defined\n");
        return -1;
    }
    socketAddress.sin_addr.s_addr = *((unsigned long*)he->h_addr);
    socketAddress.sin_port = htons(port);
    if (connect(newSocket, (struct sockaddr *)&socketAddress, sizeof(socketAddress))) {
        printf("YajnaValkya::YVSSLSocket: error connecting to server.\n");
        return -1;
    }
    
    return newSocket;
}

SSL *YVSSLSocket::enableSSL(const int socket) {
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    SSL* newSSL = SSL_new(ctx);
    if (!newSSL) {
        printf("YajnaValkya::YVSSLSocket: error creating SSL.\n");
        return NULL;
    }
    
    SSL_set_fd(newSSL, socket);
    int err = SSL_connect(newSSL);
    if (err <= 0) {
        printf("YajnaValkya::YVSSLSocket: error creating SSL connection\n", err);
        return NULL;
    }
    
    return newSSL;
}

};
