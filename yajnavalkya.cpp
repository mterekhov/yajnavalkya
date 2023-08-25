//
//  main.cpp
//  mail_sender
//
//  Created by cipher on 24.08.2023.
//

#include <iostream>
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

int main(int argc, const char * argv[]) {
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
    
    BIO_set_conn_hostname(bio, "disroot.org:993");
    BIO_do_connect(bio);
    
    char tmp[2048] = {0};
    BIO_read(bio, tmp, sizeof(tmp) - 1);

    BIO_puts(bio, "tag LOGIN yajnavalkya@disroot.org cf1f3QUNc\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);

    BIO_puts(bio, "tag LIST \"\" \"*\"\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);

    BIO_puts(bio, "tag select INBOX\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);

    BIO_puts(bio, "tag STATUS INBOX (MESSAGES)\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    std::cout << "Answer: " << tmp << std::endl;

    BIO_puts(bio, "tag FETCH 2 (BODY[HEADER.FIELDS (FROM DATE)] BODY[TEXT])\r\n");
    BIO_read(bio, tmp, sizeof(tmp) - 1);
    std::cout << "Answer: " << tmp << std::endl;
    
    return 0;
}

//bool RequestQueue(const char* serverName)
//{
//    SOCKET      hSocket = INVALID_SOCKET;
//    char        receiveBuf[512];
//    ZeroMemory(receiveBuf,512);
//    char        requestBuf[512];
//    ZeroMemory(requestBuf,512);
//    sockaddr_in sockAddr = {0};
//    bool        bSuccess = true;
//
//    //SSL
//    SSL* ssl;
//    SSL_CTX* ctx;
//
//    try
//    {
//        //Look up hostname and fill sockaddr_in structure
//        cout<< "Looking up hostname "<<serverName<<"...";
//        FillSockAddr(&sockAddr,serverName,IMAP_SERVER_PORT);
//        cout<< "found.\n";
//
//        //creating socket
//        cout<<"Creating socket..";
//        if((hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET)
//            throw exception("could not create socket!");
//        cout<<"created.\n";
//
//            //Connect to server
//        cout<<"Attempting to connect to "<< inet_ntoa(sockAddr.sin_addr)
//            <<":"<<IMAP_SERVER_PORT<<" ...";
//        if(connect(hSocket,(sockaddr*)(&sockAddr),sizeof(sockAddr))!= 0)
//            throw exception("could not connect!");
//
//        cout<<"connected\n";
//
//        ctx = SSL_CTX_new(SSLv23_client_method());
//        if(!ctx)
//            throw exception("SSL_CTX_new error");
//
//        ssl = SSL_new(ctx);
//        SSL_CTX_free(ctx);
//        if(!ssl)
//            throw exception("ssl initializing error");
//
//        SSL_set_fd(ssl,hSocket);
//        if(SSL_connect(ssl) != 1)
//            throw exception("SSL_connect error");
//
//        int reqLen;
//        int retLen;
//        cout<<"==============\n";
//        cout<<"====begin=====\n";
//        cout<<"==============\n";
//
//        retLen = SSL_read(ssl,receiveBuf,sizeof(receiveBuf));
//        if(retLen<0)
//            throw exception("SSL_read error.");
//        cout<<"S: "<<receiveBuf;
//
//        strcpy(requestBuf,"a001 CAPABILITY");
//        reqLen = strlen(requestBuf);
//        SSL_write(ssl,requestBuf,reqLen);
//        cout<<"C: "<<requestBuf<<endl;
//
//        ZeroMemory(receiveBuf,sizeof(receiveBuf));
//        retLen = SSL_read(ssl,receiveBuf,sizeof(receiveBuf));
//        if(retLen<0)
//            throw exception("SSL_read error.");
//
//        cout<<"S: "<<receiveBuf;
//    }
//    catch(exception e)
//    {
//        cout<<"Error : "<<e.what()<<endl;
//    }
//    if(hSocket != INVALID_SOCKET)
//    {
//        SSL_shutdown(ssl);
//        closesocket(hSocket);
//        SSL_free(ssl);
//    }
//
//    return bSuccess;
//}
