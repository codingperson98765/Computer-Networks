//Author: Ahmedh Shamsudeen UCID 30121541



#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include<fcntl.h>	


#define PORT_NUM 12345  
#define SIZE 1024 
#define HTTP_PORT 80
#define http_msg "CPSC441/ass1/clown2.png"
#define http_msg2 "CPSC441/ass1/clown1.png"


size_t patcnt2 = 0;
size_t patcnt3 = 0;





int echo(int);
char *strnset(const char *str, char ch, int n);
int recv_timeout(int s , int timeout, int fd);


int main()
{
	
	
	

	//Create Socket for incomming TCP connections
	int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
	int connection_fd;

	//Check to see if we failed
	if(listener_fd == -1){ perror("Failed to create socket file discriptor"); return EXIT_FAILURE; }

	
	
	struct sockaddr_in listener_sa;
	memset(&listener_sa,0,sizeof(struct sockaddr_in));
	listener_sa.sin_family = AF_INET;
	listener_sa.sin_port = htons(PORT_NUM);
	listener_sa.sin_addr.s_addr = htonl(INADDR_ANY);

	
	
	
	if(bind(listener_fd,(struct sockaddr*) &listener_sa, 
			    sizeof(struct sockaddr_in)) == -1)
	{
		perror("Failed to bind socket\n");
		return EXIT_FAILURE;
	}
	
	
	if(listen(listener_fd,5) == -1) { return EXIT_FAILURE; }
	
	
	
	struct sockaddr_in temp;
	socklen_t t = sizeof(struct sockaddr_in);
	while((connection_fd = accept(listener_fd,(struct sockaddr*)&temp,&t)) != -1)
	{

		
		int pid = fork();
		if(pid == 0) //Child
		{
			
			
			echo(connection_fd);
			close(connection_fd);
			exit(EXIT_SUCCESS);
		}
		else if(pid > 0)//Parent
		{
			close(connection_fd);
		}
		else//Error
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}


int echo(int fd)
{
	char buff[SIZE];
	ssize_t bytes;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(struct timeval));
	do{
		bytes = recv(fd,buff,SIZE,0);
		
			
		
	}while(bytes > 0);
	
	//checking for existence of jpg image or gif image
	const char happy2[15] = ".jpg";
	const char happy3[15] = ".gif";
	size_t const happyLength3 = strlen(happy2);
  	size_t const happyLength2 = strlen(happy2);
  	size_t const originalLength = strlen(buff);
  	const char * oriptr1;
  	const char * happyLength1;
	    

	for (oriptr1 = buff; happyLength1 = strstr(oriptr1, happy2); oriptr1 = happyLength1 + happyLength2)
  {
    patcnt2++;

  }
  for (oriptr1 = buff; happyLength1 = strstr(oriptr1, happy3); oriptr1 = happyLength1 + happyLength3)
  {
    patcnt3++;

  } 
  //if jpg found send get request for clown image
	if (patcnt2 != 0){
  				
				srand(time(NULL));
				
				int r = rand() % 20;

				
				
				if (r <= 10){
					sprintf(buff, "GET http://pages.cpsc.ucalgary.ca/~carey/%s HTTP/1.0\r\n\r\n", http_msg2);	
				}
				else {
				sprintf(buff, "GET http://pages.cpsc.ucalgary.ca/~carey/%s HTTP/1.0\r\n\r\n", http_msg);
				}
				
				
}
	

	
//configuring socket to be able to send GET to website	
	int new = socket(AF_INET,SOCK_STREAM,0);
	struct hostent *addr;
	addr = gethostbyname("pages.cpsc.ucalgary.ca");
	struct sockaddr_in connection_sa;
	memset(&connection_sa,0,sizeof(struct sockaddr_in));
	connection_sa.sin_family = AF_INET;
	connection_sa.sin_port = htons(HTTP_PORT);
	

	bcopy(addr->h_addr_list[0],&connection_sa.sin_addr.s_addr,addr->h_length);
	connect(new,(struct sockaddr*)&connection_sa,
			      sizeof(struct sockaddr_in));
	if(new == -1) { perror("Failed to connect"); return 1;}
	setsockopt(new,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(struct timeval));
	
	send(new,buff,sizeof(buff),0);
	bytes = 0;
	do{		      		   	   
			   int total_recv = recv_timeout(new, 4, fd);
			 		
	}while( bytes > 0 );
	
	close(new);	

	return 0;
}
	//method to replaces substring from string
	char * stringSwitch(
		char const * const buff, //buffer with everyting
		char const * const happy, //string too look for to be replaced
		char const * const replacement // string to replace with 
	) {

	//Preparing to parse data in buffer to replace Happy with Silly
	
	size_t const replacementLength = strlen(replacement);
  	size_t const happyLen = strlen(happy);
  	size_t const originalLength = strlen(buff);
	size_t patcnt = 0;
  	const char * oriptr;
  	const char * happyLength;  
	
	//counting number of occurences of the word that will be replaced
	for (oriptr = buff; happyLength = strstr(oriptr, happy); oriptr = happyLength + happyLen)
  {
    patcnt++;

  }
  //creating new string with proper length
  size_t const retlen = originalLength + patcnt * (replacementLength - happyLen);
  char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );
  if (returned != NULL){
	  char * retptr = returned;
	  for (oriptr = buff; happyLength = strstr(oriptr, happy); oriptr = happyLength + happyLen)
	  {
		size_t const skplen = happyLength - oriptr;
		strncpy(retptr, oriptr, skplen);
		retptr += skplen;
		strncpy(retptr, replacement, replacementLength);
		retptr += replacementLength;

	  }
	  strcpy(retptr, oriptr);
  }
   
	return returned;
}	

//method that recieves data in a loop with a small buffer and sends back to web browser immediately
int recv_timeout(int s , int timeout, int fd)
{
	int size_recv , total_size= 0;
	struct timeval begin , now;
	char chunk[SIZE];
	char str2[SIZE];
	double timediff;
	
	//make socket non blocking
	fcntl(s, F_SETFL, SOCK_NONBLOCK);
	
	
	while(1)
	{
		
		memset(chunk ,0 , SIZE);	//clearing buffer
		if((size_recv =  recv(s , chunk , SIZE , 0) ) < 0)
		{
			//exit if no bytes received
		}
		else
		{
			total_size += size_recv;
			if ((patcnt2 == 0 ) && (patcnt3 == 0)){
			const char replacement[10] = "Silly";
			const char happy[10] = "Happy";
			
			//if word happy is found string will be switched and send to web browser
			send(fd,stringSwitch(chunk,happy, replacement),size_recv,0);
			}
			else if (patcnt3 != 0){
				send(fd,chunk,size_recv,0);
			}
			else{
				
				
				send(fd,chunk,size_recv,0);



			}
			
		}
	}
	
	patcnt3 = 0;
	patcnt2 = 0;
	return total_size;
}
	
