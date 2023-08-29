//
//  main.cpp
//  mail_sender
//
//  Created by cipher on 24.08.2023.
//

#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "YVVerificationCode.h"
#include "YVHTTPRequestService.h"

std::string recognizeCaptcha(const std::string& filename) {
	tesseract::TessBaseAPI *tesseract = new tesseract::TessBaseAPI();
	if (tesseract->Init(NULL, "eng")) {
		printf("Tesseract failed to init");
		return "";
	}
	Pix *image = pixRead(filename.c_str());
	tesseract->SetImage(image);
	std::string result = tesseract->GetUTF8Text();

	tesseract->End();
	pixDestroy(&image);

	return result;
}

int main(int argc, const char * argv[]) {
    spcYajnaValkya::YVHTTPRequestService httpService;
    httpService.sendHTTPSRequest("");
    
//    spcYajnaValkya::YVVerificationCode verificationCode;
//    verificationCode.fetchVerificationCode();
//    
//    printf("text recognized <%s>\n", recognizeCaptcha("/Users/cipher/Desktop/captcha/captcha.jpeg").c_str());
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
