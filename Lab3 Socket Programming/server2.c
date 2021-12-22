// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>

#define PORT 5555
#define MAX_INPUT_SIZE 256

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

void *handler(void *sock)
{
    int new_socket = *((int *)sock);
    // free(sock);
    do
    {
        int n;
        // bzero(inputbuf, MAX_INPUT_SIZE);
        char inputbuf[MAX_INPUT_SIZE] = {0};
        n = read(new_socket, inputbuf, 256);

        int ans = checkOp(inputbuf);
        fflush(stdout);
        if (n < 0)
        {
            puts("error read");
            fprintf(stderr, "ERROR writing to socket\n");
            fflush(stdout);
            exit(1);
        }
        char answer[256];
        bzero(answer, MAX_INPUT_SIZE);
        sprintf(answer, "%d", ans);
        puts(answer);
        fflush(stdout);
        send(new_socket, answer, strlen(answer), 0);
    } while (1);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread, n;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char inputbuf[MAX_INPUT_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
printf("Server has started....\n");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    do
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connected\n");
        int *client = &new_socket;
         printf("forking...\n");
        int id = fork();
       
        if (id == 0)
        {
            close(server_fd);
            handler(client);
        }
    } while (1);

    printf("Hello message sent\n");
    return 0;
}