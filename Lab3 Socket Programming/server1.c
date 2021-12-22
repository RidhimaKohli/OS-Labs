#include <stdio.h> //for input output
#include <sys/types.h> //data types for system calls
#include <sys/socket.h> //sockets
#include <stdlib.h> 
#include <netinet/in.h> //for network domains
#include <string.h>
#include <unistd.h>
#include <netdb.h>
// #include <pthread.h>

#define BACKLOG 0

int checkOp(char* str){


   const char plus = '+';
   const char min = '-';
   const char mult = '*';
   const char div = '/';
int opVal;
   char *ret;
   const char *operator;
   if( (ret = strchr(str, plus)) != NULL){
       opVal = 1;
       operator="+";
       
   }
   else if( (ret=strchr(str,min))!=NULL){
        opVal = 2; 
        operator="-";
     
   }
      else if( (ret=strchr(str,mult))!=NULL){
           opVal = 3;
           operator="*";

   }
      else if( (ret=strchr(str,div))!=NULL){
           opVal = 4;
           operator="/";
     
   }

// printf("string : %s\n",str);

   const char *op = &ret[0];
   const char *s1 =op;
   char *token;
   
// printf("op : %c\n",s1[0]);

   token = strtok(str, operator);
//    printf("token : %s\n",token);
   int a;
   int b;
   int flag=0;
   while( token != NULL ) {
 
    if(flag==0){
        a=atoi(token);
        flag=1;
    }
    else{
        b=atoi(token);
    }
      token = strtok(NULL, operator);
   }


   int c;
switch (opVal)
{
case 1:
    c=a+b;
    break;
case 2:
    c=a-b;
    break;
case 3:
    c=a*b;
    break;
case 4:
    c=a/b;
    break;

default:
    break;
}

return c;

}


int main(int argc , char* argv[]){

if(argc<2){
    printf("Please provide port number\nTerminating program...\n");
    exit(1);
}
int sockfd , new_sockfd , port , n;
char buffer[255]; //store msgs here

struct sockaddr_in serverAddr , clientAddr;
socklen_t clientaddr_size;
sockfd = socket(AF_INET,SOCK_STREAM,0);

if(sockfd<0){
    perror("couldn't open socket\n");
    exit(1);
}

bzero((char*)&serverAddr,sizeof(serverAddr)); //erases
port = atoi(argv[1]);
serverAddr.sin_family = AF_INET;
serverAddr.sin_addr.s_addr = INADDR_ANY;
serverAddr.sin_port = htons(port);
if ( bind(sockfd, (struct sockaddr *) &serverAddr,sizeof(serverAddr) ) <0){
    perror("Failed to bind\n");
    exit(1);
}

listen(sockfd,BACKLOG);
clientaddr_size = sizeof(clientAddr);
printf("Waiting for connection....\n");

// new_sockfd = accept(sockfd, (struct sockaddr *) &clientAddr , &clientaddr_size);
// if(new_sockfd<0){
//     perror("Failed to accept\n");
//     exit(1);
// }
// printf("Connected\n");



while(1){

new_sockfd = accept(sockfd, (struct sockaddr *) &clientAddr , &clientaddr_size);
if(new_sockfd<0){
    perror("Failed to accept\n");
    exit(1);
}

printf("Connected to new client\n");

close(sockfd);



    while(1){
            
                    bzero(buffer,255);
                    n=read(new_sockfd , buffer ,255);
                    if(read<0){
                        perror("Failed to read\n");
                        exit(1);
                    }
                    
                    int i = strncmp("bye",buffer,3);
                    if(i==0){
                                    close(new_sockfd);
                                    break;
                                }
                    printf("Client's ques : %s\n",buffer);
                    int ans;
                    ans=checkOp(buffer);
                    
                    bzero(buffer,255);
                    sprintf(buffer, "%d", ans);

                    n = write(new_sockfd , buffer , strlen(buffer));

                    if(n<0){
                        perror("Failed to write\n");
                        exit(1);
                    }


    }


}



close(new_sockfd);
close(sockfd);
    return 0;
}