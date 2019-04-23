/*
 * Server code for UDP socket 
 * 1. The server starts and waits for filename
 * 2. The client sends a filename
 * 3. The server receives filename.
 *     If file present,
 *     server starts reading the file, and continues to send a buffer filled with file contents encrypted until file-end is reached.
 * 4. End is marked by EOF
 * 5. File is received as buffers until EOF is received. Then it is decrypted
 * 6. If not preset, a file not found is sent
 *   */


#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define secureKey 'S'
#define sendRecvFlag 0
#define noFile "File Not Found!"

// clear the buffer
//
void clearBuf(char* b) {
    int i;
    for(i = 0; i < NET_BUF_SIZE; i++)
            b[i] = '\0';
 }

//function to encrypt data
//
char Cipher(char ch) {
    return ch ^ secureKey;
}
//function to send file
//
// funtion sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, noFile); 
        len = strlen(noFile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = Cipher(buf[i]); 
        return 1; 
    } 
  
    char ch, ch2; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
        ch2 = Cipher(ch); 
        buf[i] = ch2; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
}  

//main function driver code
//
int main() {
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf[NET_BUF_SIZE];
    FILE* fp;

    //socket ()
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    if (sockfd < 0)
        printf("\n Nu am primit file descriptor !!\n");
    else
        printf("\n Am primit un file descriptor %d \n", sockfd);
    //bind ()
    //
    if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0 )
        printf("\n Conexiunea a fost stabilita!\n");
    else
        printf("\n Conexiunea nu a fost stabilita!\n");
    while(1) {
        printf("\n Waiting for file name ...\n");
        //recive file name
       clearBuf(net_buf);
       
       nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendRecvFlag, (struct sockaddr*)&addr_con, &addrlen);

       fp = fopen(net_buf, "r");
       printf("\n Am primit numele fisierului: %s \n", net_buf);
       if( fp == NULL)
           printf("\n Nu am reusit sa deschid fisierul! \n");
       else
           printf("\n Am reusit sa deshchid fisierul! \n");
      while (1) {
          //process
          //
          if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
              sendto(sockfd, net_buf, NET_BUF_SIZE, sendRecvFlag, (struct sockaddr*)&addr_con, addrlen);
              break;
           }
          //send
          sendto(sockfd, net_buf, NET_BUF_SIZE, sendRecvFlag, (struct sockaddr*)&addr_con, addrlen);
          clearBuf(net_buf);
      }
      if (fp != NULL)
          fclose(fp);
    }
    return 0;
}
