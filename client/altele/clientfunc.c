#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
:exit
#include <unistd.h> 
#include "client.h"

// funtion to clear buffer 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < NET_BUF_SIZE; i++) 
        b[i] = '\0'; 
} 
  
// function for decryption 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// function to receive file 
int recvFile(char* buf, int s) 
{ 
    int i; 
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = buf[i]; 
        ch = Cipher(ch); 
        if (ch == EOF) 
            return 1; 
        else
            printf("%c", ch); 
    } 
    return 0; 
}
int createConnection(sockaddr_in *addr_con) {

}
void sendFile() {
    struct sockaddr_in addr_con; 
    int addrlen = sizeof(addr_con); 
    addr_con.sin_family = AF_INET; 
    addr_con.sin_port = htons(PORT_NO); 
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    FILE* fp; 
    char net_buf[NET_BUF_SIZE]; 
    int sockfd, nBytes; 
 
    sockfd = socket(AF_INET, SOCK_DGRAM,IP_PROTOCOL);
    if (sockfd < 0) 
        printf("\nfile descriptor not received!!\n"); 
    else
        printf("\nfile descriptor %d received\n", sockfd); 
  
    while (1) { 
        printf("\nPlease enter file name to receive:\n"); 
        scanf("%s", net_buf); 
        sendto(sockfd, net_buf, NET_BUF_SIZE, 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 
  
        printf("\n---------Data Received---------\n"); 
  
        while (1) { 
            // receive 
            clearBuf(net_buf); 
            nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, 
                              sendrecvflag, (struct sockaddr*)&addr_con, 
                              &addrlen); 
  
            // process 
            if (recvFile(net_buf, NET_BUF_SIZE)) { 
                break; 
            } 
        } 
        printf("\n-------------------------------\n"); 
    } 
   // return 0; 
}
