#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define MAX 1024
#define KEY 130

typedef struct message
{
	long type;
	char data[MAX];

}message;
int counter=0,iterations=0,flag=0,i=0;       
int root_pid;  

int noi,prior,sleeptime,prob,msqid;
pid_t proc_pid;

void notify();
void suspend();
void iterate();

message msg;   
int flg=0;

int main( int argc,char *argv[]  ){
	srand(time(NULL));    
	key_t key=130;
	
	int pid;
	char buff[MAX],buffer[MAX];
	proc_pid=getpid();
	
	printf("PID %d\n",proc_pid);

	signal(SIGUSR1,suspend);
	signal(SIGUSR2,notify);


	if(argc!=5){
		printf("error in the number of passed parameters\n");
		exit(0);
	}

	noi=atoi(argv[1]);
	prior=atoi(argv[2]);
	prob=atoi(argv[3]);
	sleeptime=atoi(argv[4]);

	printf("In round %d prior=%d prob=%d slptime=%d \n",noi,prior,prob,sleeptime);
	iterations=noi;   

	msqid=msgget(key,IPC_CREAT|0644);
	sprintf(buffer,"%d ",proc_pid);   
	sprintf(buff,"%d",prior);        
	strcat(buffer,buff);
	strcat(msg.data,buffer);
	printf("sending message %s\n",msg.data);
	msg.type=1;         
	int stat=msgsnd(msqid,&msg,MAX,0);
	if(stat<0)
		printf("failed\n");


	msgrcv(msqid,&msg,MAX,proc_pid,MSG_NOERROR);
	
	root_pid=atoi(msg.data);
	printf("Sched PID %d \n",root_pid);




	pause(); 
	flag=1;
	

	
	
	iterate();


	return 0;
}


void iterate()
{
	printf("starting iterations\n");
	int pb,stat;
	for(i=counter;i<iterations;i++,counter++)
	{
		
		while(flg==1)
		{
			sleep(1);
		}

		pb=1 + rand()%100;
		if(prob > 0.3)
		{

			kill(root_pid,SIGUSR1); 

			printf("PID: %d Going for IO  at i =%d value of iter %d \n",proc_pid,i,iterations );
			sleep(sleeptime);

	
			sprintf(msg.data,"%d",proc_pid);
			msg.type=2;
			stat=msgsnd(msqid,&msg,MAX,0);
			if(stat==-1)
				printf("not sent\n");

			pause();
			printf("waking\n");
			
		}

	}

	if(i==iterations)
	{
	
		kill(root_pid,SIGUSR2); 
	}
}

void notify()
{
	printf("signal received \n");
	flg=0;
	signal(SIGUSR1,suspend);
}

void suspend()
{
	signal(SIGUSR2,notify);
	printf("being suspended by sched\n");
	counter=i;
	flg=1;
}