//Header with functions for client
//
// 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 15050 
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 

char Cipher(char ch); 

void clearBuf(char* b);

int recvFile(char* buf, int s);
void sendFile();
int createConnection();
