//reference Steves, Unix Network programming (3erd)
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define SA struct sockaddr
#define INTERRUPTED_BY_SIGNAL (errno == EINTR)

/* signal handlers */
typedef void Sigfunc(int);
struct hostent *Gethostbyname (const char *hostname);
int Socket( int family, int type, int protocol);
int Accept(int listent_sockfd, SA *cliaddr, socklen_t *addrlenp);
int Select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);

char * sock_ntop(const struct sockaddr *sa, socklen_t salen);
char * Sock_ntop(const struct sockaddr *sa, socklen_t salen);
char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
char * Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
char * Fgets(char *ptr, int n, FILE *stream);

pid_t Fork(void);
FILE* Fopen(const char *filename, const char *mode);
Sigfunc *signal(int signo, Sigfunc *func);
Sigfunc *signal(int signo, Sigfunc *funct);


void Send(int fd, void *bufptr, size_t nbytes, int flags);
void Sendto(int fd, void *bufptr, size_t nbytes, int flags, const SA *sa, socklen_t salen);
void Sendn(int fd, void *ptr, size_t nbytes, int flags);
void Inet_aton(const char *strptr, struct in_addr *addrptr);
void Inet_pton(int af, const char *strptr, void *addrptr);
void Inet_ntop(int af, const void *addrptr, char *strptr, size_t length);
void Bind(int sockfd, const SA* myaddr, socklen_t myaddrlen);
void Listen(int sockfd, int backlog);
void Connect(int sockfd, const SA *srvaddr, socklen_t addrlen);
void Close(int fd);
void Shutdown(int fd, int howto);
void Writen(int fd, void *ptr, size_t nbytes);
void Write(int fd, void *bufptr, size_t nbytes);
void Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrp);
void Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
void Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr);
void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen); 
void Fclose(FILE *fp);
void Fputs(const char *ptr, FILE *stream);
void showAddr(char *str, struct sockaddr_in *a);

ssize_t Read(int fd, void *buffptr, size_t nbytes);
ssize_t Recv(int fd, void *bufptr, size_t nbytes, int flags);
ssize_t Recvfrom(int fd, void *bufptr, size_t nbytes, int flgas, SA *sa, socklen_t *salenptr);


ssize_t readn(int fd, void *vptr, size_t n);
ssize_t Readn(int fd, void *ptr, size_t nbytes);
ssize_t readline_buffered(int fd, void *ptr, size_t maxlen);
ssize_t readline_unbuffered(int fd, void *vptr, size_t maxlen);
ssize_t Readline_buffered(int fd, void *ptr, size_t maxlen);
ssize_t Readline_unbuffered(int fd, void *ptr, size_t maxlen);
ssize_t readline(int fd, void *ptr, size_t maxlen);
ssize_t Readline(int fd, void *ptr, size_t maxlen);
ssize_t writen(int fd, const void *ptr, size_t n);
ssize_t sendn(int fd, const void *vptr, size_t n, int flags);





