/*
 * 	File Server.c
 *	ECHO UDP SERVER with the following features:
 *      - Gets port from arguments 
 *      - SEQUENTIAL: serves one request at a time
 * 	- FUNCTIONS: > recognise the ACK requests and reply with a strings
 * 		     > print all the other received requests
 */


#include    <stdlib.h>
#include    <string.h>
#include    <inttypes.h>
#include    "../../utils/errlib.h"
#include    "../../utils/socks.h"

#define MAXSENDERS	10	  /* Maximum # of senders accepted */
#define BUFLEN		65536 /* Maximum UDP datagram length */
#define ECHOTIME 	10    /* TIMEOUT of server regards (seconds) */
char	 			buf[BUFLEN];		/* reception buffer */

/* GLOBAL VARIABLES */
char *prog_name;
struct sockaddr_in senders[MAXSENDERS];

/* MY METHODS */
int delete_cliet_in_lists(struct sockaddr_in  from, int *count);
int insert_cliet_in_lists(struct sockaddr_in  from, int *count);
int cliet_in_lists(struct sockaddr_in  from, int count);

int eq_sockkaddr_in(struct sockaddr_in a1,struct sockaddr_in a2);
//void set_sockkaddr_in(struct sockaddr_in a1,struct sockaddr_in a2);


int main(int argc, char *argv[])
{
    uint16_t 			lport_n, lport_h;	/* port where server listens */
    int					s;
    int					len, s_count;
    socklen_t			addrlen;
    struct sockaddr_in	saddr, from;

    prog_name = argv[0];

    if (argc != 2) {
		printf("Folosire: %s <port>\n", prog_name);
		exit(1);
    }

    /* get server port number */
    if (sscanf(argv[1], "%" SCNu16, &lport_h)!=1)
    	err_sys("Portul nu este corect");
    lport_n = htons(lport_h);

    /* create the socket */
    printf(" creez socketul \n");
    s = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("gata, socket nr:  %u\n",s);

    /* bind the socket to all local IP addresses */
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = lport_n;
    saddr.sin_addr.s_addr = INADDR_ANY;
    showAddr("Ma conectez la adresa:", &saddr);
    Bind(s, (struct sockaddr *) &saddr, sizeof(saddr));
    printf("done.\n");
    
    s_count=0;
	printf("\nIntru in bucla princiapala...\n");
    /* main server loop */
    for (;;)
	{
		addrlen = sizeof(struct sockaddr_in);
		
		len=recvfrom(s, buf, BUFLEN-1, 0, (struct sockaddr *)&from, &addrlen);
		buf[len]=0;
		if (len != -1)
		{
			
			if(strcmp(buf,"ACK")==0)
			{
				if(insert_cliet_in_lists(from, &s_count)==-1)
					continue;
					
				printf("Client nou, raspund: ....-> ");
				strcpy(buf, "Salut Utilizator:");
				len = strlen(buf);
				if (sendto(s, buf, len, 0, (struct sockaddr *)&from, addrlen) != len)
				{
					printf("Eroare la scriere\n");
				}else
				{
					printf("Done\n");
				}
				continue;
			}
			
			if(strcmp(buf,"CLOSE")==0)
			{
				printf("un utilizator a parasit canalul\n");
				delete_cliet_in_lists(from, &s_count);
				continue;
			}
		
			if(cliet_in_lists(from, s_count)==-1)
				printf("Nu e in lista, mesajul a fost aruncat\n");
			else
			{
				buf[len] = '\0';
				showAddr("Am primit mesaj de la:", &from);
				printf(": [%s]\n", buf);
			}
			
		}else{
			printf("Eroare in mesagerie!\n");
		}
		
    }    
    exit(1);
}



int delete_cliet_in_lists(struct sockaddr_in  from, int *count){
	
	if (*count == 0)
		return -1;
	else
	{
		int i;
		for(i=0; i<*count; i++)
			if(eq_sockkaddr_in(senders[i],from)==0)
			{
				for(;i<*count;i++)
					senders[i]=senders[i+1];
				break;
				
			}
		(*count)--;
	}
	return 0;
	
}

int insert_cliet_in_lists(struct sockaddr_in  from, int *count){
	
	if ((*count == MAXSENDERS) || (cliet_in_lists(from, *count)==0))
		return -1;
	else{
		senders[*count]=from;
		(*count)++;
	}
	return 0;
	
}

int cliet_in_lists(struct sockaddr_in  from, int count){

	int i;
	for(i=0; i<count; i++)
		if(eq_sockkaddr_in(senders[i],from)==0)
		{
			return 0;
		};
	return -1;
}

int eq_sockkaddr_in(struct sockaddr_in a1,struct sockaddr_in a2){
	if((a1.sin_addr.s_addr == a2.sin_addr.s_addr) && 
		(a1.sin_port == a2.sin_port))
		return 0;
	else
		return -1;
}
