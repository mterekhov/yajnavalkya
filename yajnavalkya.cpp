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

#include "cpp/opportunisticsecuresmtpclient.hpp"
#include "cpp/plaintextmessage.hpp"
#include <openssl/ssl.h>

int main(int argc, const char * argv[]) {
    BIO *bio;
    SSL *ssl;
    SSL_CTX *ctx;
    int bytesRecv = 0;
    
    
    CRYPTO_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
    SSL_library_init(); // Initialize OpenSSL's SSL libraries
    SSL_load_error_strings(); // Load SSL error strings
    ERR_load_BIO_strings(); // Load BIO error strings
    OpenSSL_add_all_algorithms(); // Load all available encryption algorithms
    
    ctx = SSL_CTX_new(SSLv23_client_method());
    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    
    BIO_set_conn_hostname(bio, "imap.gmail.com:993");
    BIO_do_connect(bio);
    
    char tmp[2000];
    std::cout << "Bytes received: " << BIO_read(bio, tmp, 1000) << std::endl; // Receive 68 bytes (server welcome string)
    
    std::cout << std::endl << "LOGIN:" << std::endl;
    BIO_puts(bio, "tag LOGIN user@gmail.com password\r\n");
    std::cout << "Bytes received: " << BIO_read(bio, tmp, sizeof(tmp) - 1) << std::endl;
    std::cout << "Answer: " << tmp << std::endl;
    std::cout << std::endl << "LIST:" << std::endl;
    BIO_puts(bio, "tag LIST \"\" \"*\"\r\n");
    std::cout << "Bytes received: " << BIO_read(bio, tmp, sizeof(tmp) - 1) << std::endl;
    std::cout << "Answer: " << tmp << std::endl;
    std::cout << std::endl << "INBOX:" << std::endl;
    BIO_puts(bio, "tag select INBOX\r\n");
    std::cout << "Bytes received: " << BIO_read(bio, tmp, sizeof(tmp) - 1) << std::endl;
    std::cout << "Answer: " << tmp << std::endl;
    std::cout << std::endl << "STATUS:" << std::endl;
    BIO_puts(bio, "tag STATUS INBOX (MESSAGES)\r\n");
    std::cout << "Bytes received: " << BIO_read(bio, tmp, sizeof(tmp) - 1) << std::endl;
    std::cout << "Answer: " << tmp << std::endl;
    
    BIO_puts(bio, "tag FETCH 4224 BODY[]\r\n");
    std::cout << "Bytes received: " << BIO_read(bio, tmp, sizeof(tmp) - 1) << std::endl;
    std::cout << "Answer: " << tmp << std::endl;
    
    do{ //There I am trying to receive all message
        std::cout << "i=" << i << std::endl;
        memset(tmp, 0, sizeof(tmp));
        bytesRecv = BIO_read(bio, tmp, sizeof(tmp)); //I am expecting to get 0 in the end of message, but instead function blocks
        std::cout << "Bytes received: " << bytesRecv  << std::endl;
        std::cout << "Answer: " << tmp << std::endl;
        system("pause");
    }
    while(bytesRecv);
    return 0;
}

void sendingEmail() {
    jed_utils::cpp::OpportunisticSecureSMTPClient client("disroot.org", 587);
    client.setCredentials(jed_utils::cpp::Credential("yajnavalkya@disroot.org", "cf1f3QUNc"));
    try    {
        jed_utils::cpp::PlaintextMessage msg(jed_utils::cpp::MessageAddress("yajnavalkya@disroot.org", "Test Address Display"),
                                             { jed_utils::cpp::MessageAddress("m.terekhov@icloud.com", "Another Address display") },
                                             "This is a test (Subject)",
                                             "Hello\nHow are you?");
        
        int err_no = client.sendMail(msg);
        if (err_no != 0) {
            std::cerr << client.getCommunicationLog() << '\n';
            std::string errorMessage = client.getErrorMessage(err_no);
            std::cerr << "An error occurred: " << errorMessage
            << " (error no: " << err_no << ")" << '\n';
            return 1;
        }
        std::cout << client.getCommunicationLog() << '\n';
        std::cout << "Operation completed!" << std::endl;
    }
    catch (std::invalid_argument &err) {
        std::cerr << err.what() << std::endl;
    }
}

void customSocket() {
    const int port = 25;
    const std::string hostAddress = "smtp.yandex.ru";
    int fd;
    socklen_t n;
    
    int smtpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (smtpSocket == -1) {
        printf(">>> failed to create new socket\n");
        return;
    }
    
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(port);
    
    struct hostent *host = gethostbyname(hostAddress.c_str());
    socketAddress.sin_addr.s_addr = *(host->h_addr);
    
    printf(">>> connecting to server ...\n");
    if (connect(smtpSocket, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) == -1) {
        printf(">>> failed to connect to server\n");
        return;
    }
    printf(">>> connection successfull\n");
    
    char buffer[1024] = {0};
    int bytesReceived = recv(smtpSocket, buffer, sizeof(buffer), 0);
}

//// Insist on at least Winsock v1.1
//const VERSION_MAJOR = 1;
//const VERSION_MINOR = 1;
//
//#define CRLF "\r\n"                 // carriage-return/line feed pair
//
//void ShowUsage(void)
//{
//  std::cout << "Usage: SENDMAIL mailserv to_addr from_addr messagefile" << std::endl
//       << "Example: SENDMAIL smtp.myisp.com rcvr@elsewhere.com my_id@mydomain.com message.txt" << std::endl;
//
//  exit(1);
//}
//
//// Basic error checking for send() and recv() functions
//void Check(int iStatus, char *szFunction)
//{
//  if((iStatus != SOCKET_ERROR) && (iStatus))
//    return;
//
//  cerr << "Error during call to " << szFunction << ": " << iStatus << " - " << GetLastError() << std::endl;
//}
//
//int main(int argc, char *argv[])
//{
//  int         iProtocolPort        = 0;
//  char        szSmtpServerName[64] = "";
//  char        szToAddr[64]         = "";
//  char        szFromAddr[64]       = "";
//  char        szBuffer[4096]       = "";
//  char        szLine[255]          = "";
//  char        szMsgLine[255]       = "";
//  SOCKET      hServer;
//  WSADATA     WSData;
//  LPHOSTENT   lpHostEntry;
//  LPSERVENT   lpServEntry;
//  SOCKADDR_IN SockAddr;
//
//  // Check for four command-line args
//  if(argc != 5)
//    ShowUsage();
//
//  // Load command-line args
//  lstrcpy(szSmtpServerName, argv[1]);
//  lstrcpy(szToAddr, argv[2]);
//  lstrcpy(szFromAddr, argv[3]);
//
//  // Create input stream for reading email message file
//  ifstream MsgFile(argv[4]);
//
//  // Attempt to intialize WinSock (1.1 or later)
//  if(WSAStartup(MAKEWORD(VERSION_MAJOR, VERSION_MINOR), &WSData))
//  {
//    std::cout << "Cannot find Winsock v" << VERSION_MAJOR << "." << VERSION_MINOR << " or later!" << std::endl;
//
//    return 1;
//  }
//
//  // Lookup email server's IP address.
//  lpHostEntry = gethostbyname(szSmtpServerName);
//  if(!lpHostEntry)
//  {
//    std::cout << "Cannot find SMTP mail server " << szSmtpServerName << std::endl;
//
//    return 1;
//  }
//
//  // Create a TCP/IP socket, no specific protocol
//  hServer = socket(PF_INET, SOCK_STREAM, 0);
//  if(hServer == INVALID_SOCKET)
//  {
//    std::cout << "Cannot open mail server socket" << std::endl;
//
//    return 1;
//  }
//
//  // Get the mail service port
//  lpServEntry = getservbyname("mail", 0);
//
//  // Use the SMTP default port if no other port is specified
//  if(!lpServEntry)
//    iProtocolPort = htons(IPPORT_SMTP);
//  else
//    iProtocolPort = lpServEntry->s_port;
//
//  // Setup a Socket Address structure
//  SockAddr.sin_family = AF_INET;
//  SockAddr.sin_port   = iProtocolPort;
//  SockAddr.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
//
//  // Connect the Socket
//  if(connect(hServer, (PSOCKADDR) &SockAddr, sizeof(SockAddr)))
//  {
//    std::cout << "Error connecting to Server socket" << std::endl;
//
//    return 1;
//  }
//
//  // Receive initial response from SMTP server
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() Reply");
//
//  // Send HELO server.com
//  sprintf(szMsgLine, "HELO %s%s", szSmtpServerName, CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() HELO");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() HELO");
//
//  // Send MAIL FROM: <sender@mydomain.com>
//  sprintf(szMsgLine, "MAIL FROM:<%s>%s", szFromAddr, CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() MAIL FROM");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() MAIL FROM");
//
//  // Send RCPT TO: <receiver@domain.com>
//  sprintf(szMsgLine, "RCPT TO:<%s>%s", szToAddr, CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() RCPT TO");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() RCPT TO");
//
//  // Send DATA
//  sprintf(szMsgLine, "DATA%s", CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() DATA");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() DATA");
//
//  // Send all lines of message body (using supplied text file)
//  MsgFile.getline(szLine, sizeof(szLine));             // Get first line
//
//  do         // for each line of message text...
//  {
//    sprintf(szMsgLine, "%s%s", szLine, CRLF);
//    Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() message-line");
//    MsgFile.getline(szLine, sizeof(szLine)); // get next line.
//  } while(MsgFile.good());
//
//  // Send blank line and a period
//  sprintf(szMsgLine, "%s.%s", CRLF, CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() end-message");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() end-message");
//
//  // Send QUIT
//  sprintf(szMsgLine, "QUIT%s", CRLF);
//  Check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() QUIT");
//  Check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() QUIT");
//
//  // Report message has been sent
//  std::cout << "Sent " << argv[4] << " as email message to " << szToAddr << std::endl;
//
//  // Close server socket and prepare to exit.
//  closesocket(hServer);
//
//  WSACleanup();
//
//  return 0;
//}
