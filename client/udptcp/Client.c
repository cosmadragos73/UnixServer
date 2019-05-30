#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 1024 /*max text line length*/
#define LISTENQ 8 /*maximum number of client connections*/

struct message
{
    uint8_t type;
    uint16_t length;
    char msg[MAXLINE];
};

int main (int argc, char **argv)
{
	if(argc!=2)
    {
        perror("Folosire: ./client <port-number>\n");
        exit(1);
    }
    int listenfd, connfd,serv_port = atoi(argv[1]);
    struct message msg;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0)
    {
        perror("Nu am reusit sa creez socketul tcp\n");
        exit(2);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serv_port);
    if (bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)
    {
        perror("Problema la legare cu socketul tcp\n");
        exit(3);
    }
    if (listen (listenfd, LISTENQ)<0)
    {
        perror("Problema in ascultarea pe socketul tcp\n");
        exit(4);
    }
    
    int c_no = 0;
    
    printf("Serverul ruleaza...astept o conexiune\n");
    printf("Client No \tChild PID \tClient IP \tTCP/UDP \tClient Port \tAlte informtii\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    for ( ; ; )
    {
        clilen = sizeof(cliaddr);
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        //printf("Received request...\n");
	
	c_no++;
        if ( (childpid = fork ()) == 0 )  //Child process
        {
	    
            //printf("%d\t%d\tcreated for dealing with client requests\n", c_no, get_pid());
            close (listenfd);
            if( recv(connfd, &msg, MAXLINE,0) == 0)
            {
                perror(" Clientul a terminat conexiunea\n");
                exit(5);
            }
            
            printf("%d\t\t%d\t\t%s\tTCP\t\t%d\t\tReceived Message of Type: %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg.type);
            //printf("Message type received from the client: %d\n",msg.type);
	    
	    servaddr.sin_port = htons(0);
	    if ((listenfd = socket (AF_INET, SOCK_DGRAM, 0)) <0)
	    {
		    perror(" Problema in crearea socketului udp\n");
		    exit(2);
	    }
	    if (bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	    {
		    perror("Problema in legarea cu socketul udp\n");
		    exit(3);
	    }
	    struct sockaddr_in localAddress;
	    socklen_t addressLength = sizeof localAddress;
	    getsockname(listenfd, (struct sockaddr*)&localAddress,&addressLength);
	    
	    printf("%d\t\t%d\t\t%s\t---\t\t%d\t\tUDP Port Assigned:\t  %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), (int) ntohs(localAddress.sin_port));
	    //printf("Server udp port: %d\n", (int) ntohs(localAddress.sin_port));
	    msg.type = 2;
			
            msg.length = ntohs(localAddress.sin_port);
            send(connfd, &msg, sizeof(msg), 0);
	    
	    printf("%d\t\t%d\t\t%s\tTCP\t\t%d\t\tTipul mesajului trimis:\t  %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg.type);
	    
	    printf("%d\t\t%d\t\t%s\t---\t\t%d\t\tConexiunea TCP Inchisa:\t  %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), close(connfd));
	    //printf("Sent message of type %d\n",msg.type);
            //printf("close: %d\n",close(connfd));
			
	    //UDP
	    recvfrom(listenfd,&msg,sizeof(msg),0,(struct sockaddr *)&cliaddr, &clilen);
	    //printf("Receiving from (%s , %d) : ",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	    msg.msg[msg.length]='\0';
	    //printf("Message type received from the client: %d\n",msg.type);
	    //printf("\t\tMessage: %s\n",msg.msg);
	    printf("%d\t\t%d\t\t%s\tUDP\t\t%d\t\t Am primit un mesaj de tipul: %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg.type);
	    printf("%d\t\t%d\t\t%s\tUDP\t\t%d\t\t Am primit mesajul:\t  %s\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg.msg);
	    
	    msg.type=4;
	    sendto(listenfd, &msg, sizeof(msg), 0,(struct sockaddr *)&cliaddr, clilen);
	    //printf("Sent message of type %d\n",msg.type);
	    printf("%d\t\t%d\t\t%s\tUDP\t\t%d\t\tAm trimis un mesaj de tipul:\t  %d\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg.type);
	    printf("%d\t\t%d\t\t%s\t---\t\t%d\t\tChild Terminat:\t  Succes\n", c_no, getpid(), inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	    fflush(stdout);
	    
	    return 0;
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}
