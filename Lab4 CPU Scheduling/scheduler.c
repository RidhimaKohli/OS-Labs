#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define MAX 1024
#define KEY 130

int done = 0;
int count = 0;
int thread_ids[4] = {0, 1, 2, 3};
int thread_runtime[4] = {0, 5, 4, 1};
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t)
{
	int i;
	long my_id = (long)t;
	long run_time = thread_runtime[my_id];
	if (my_id == 2 && done == 0)
	{
		for (i = 0; i < 5; i++)
		{
			if (i == 4)
			{
				done = 1;
			}
			pthread_mutex_lock(&count_mutex);
			count++;

			if (count == COUNT_LIMIT)
			{
				pthread_cond_signal(&count_threshold_cv);
				printf("inc_count(): thread %ld, count = %d  Threshold reached.\n",
					   my_id, count);
			}
			printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
			pthread_mutex_unlock(&count_mutex);
		}
	}

	if (my_id == 3 && done == 1)
	{
		for (i = 0; i < 4; i++)
		{
			if (i == 3)
			{
				done = 2;
			}
			pthread_mutex_lock(&count_mutex);
			count++;

			if (count == COUNT_LIMIT)
			{
				pthread_cond_signal(&count_threshold_cv);
				printf("inc_count(): thread %ld, count = %d  Threshold reached.\n",
					   my_id, count);
			}
			printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
			pthread_mutex_unlock(&count_mutex);
		}
	}

	if (my_id == 4 && done == 2)
	{
		for (i = 0; i < 8; i++)
		{
			pthread_mutex_lock(&count_mutex);
			count++;
			if (count == COUNT_LIMIT)
			{
				pthread_cond_signal(&count_threshold_cv);
				printf("inc_count(): thread %ld, count = %d  Threshold reached.\n",
					   my_id, count);
			}
			printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
			pthread_mutex_unlock(&count_mutex);
		}
	}
	pthread_exit(NULL);
}

void *watch_count(void *t)
{
	long my_id = (long)t;

	printf("Starting watch_count(): thread %ld\n", my_id);
	pthread_mutex_lock(&count_mutex);
	if (count < COUNT_LIMIT)
	{
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("watch_count(): thread %ld Condition signal received.\n", my_id);
		count += 125;
		printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
	}
	pthread_mutex_unlock(&count_mutex);
	pthread_exit(NULL);
}

void *trythis(void *sock)
{
	pthread_mutex_lock(&lock);
	int new_socket = *((int *)sock);

	int n;

	char inputbuf[256] = {0};
	fflush(stdout);
	unsigned long i = 0;
	counter += 1;
	printf("\n Job %d has started\n", counter);
	n = read(new_socket, inputbuf, 256);
	printf("%s", inputbuf);
	sleep(3);
	printf("\n Job %d has finished\n", counter);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int flag=0;  // = 0 for rr , else 1

int pid_pr;

int termination=0;
typedef struct message
{

	long type;
	char data[MAX];

}message;
double a,b,c;
double a1=0,b1=0,c1=0;
struct node
{
	int pid;
	int priority;
	double total_runtime;
	struct node *next;
};
typedef struct tim
{
	int flag;//
	clock_t intime; //
	clock_t ftime; //
	clock_t outtime;//
}tim;
tim aa[100000];
struct node tempnode;
struct node running2;
clock_t start,end;
int flg=0;
double cpu_time_used;

struct node* readyq=NULL;
struct node* waitingq=NULL;

message msg;
int tmquanta,msqid,killed=0,io=0;
void iorequest(); 
void terminate(); 
void append(struct node** head_ref);
struct node running; 
void deletefirstNode(struct node **head_ref);
void deleteNode(struct node **head_ref, int key);
int getmin(struct node *nod);
void check();

void printList(struct node *node)
{
	printf("intial list :- \n");
  while (node != NULL)
  {
     printf(" %d ", node->pid);
     node = node->next;
  }

  printf("\n");
}
int main(int argc,char *argv[])
{
	key_t key=130;
	//int msqid;

	char buff[MAX],buffer[MAX];

	if(argc!=2)
	{
		printf("Invalid number of arguements\n");
		exit(0);	
	}


	if(strcmp(argv[1],"RR")==0)
	{
		flag=0;
		tmquanta=1000;
	}
	else if(strcmp(argv[1],"PR")==0)
	{
		flag=1;
		tmquanta=2000;
	}	
	else
	{
		printf("Invalid Entry please try again..\n");
		exit(0);
	}


	signal(SIGUSR1,iorequest);
	signal(SIGUSR2,terminate); 
	msqid=msgget(key,IPC_CREAT|0644);

	char *token;
	char s[2]=" ";
	int pid;
	int priority;
	int i,ko=0;


	printf("starting sched in 5sec \n");
	sleep(5);
	printf("scheduler start\n");
	if(flag==0)
	{
		for(i=0;i<4;i++)
		{
		
			msgrcv(msqid,&msg,MAX,1,MSG_NOERROR);
			token=strtok(msg.data,s);
			pid=atoi(token);
			token=strtok(NULL,s);
			pid_pr = pid;
			priority=atoi(token);
			
			printf("In RR sched.c pid %d priority %d \n",pid,priority);

			tempnode.pid=pid;
			tempnode.priority=priority;
			tempnode.total_runtime=0;
			aa[pid].flag=0;
			aa[pid].intime=clock();

			append(&readyq);
			sprintf(msg.data,"%d",getpid());
			msg.type=pid;
			int stat=msgsnd(msqid,&msg,MAX,0);

			if(stat==-1)
				printf("unable to send\n");

			if(stat>=0)
				printf("message sent to pid %d containing type %ld and data %s \n",pid,msg.type,msg.data);
		}

		printList(readyq);

		sleep(7);
	

		while(termination!=4) 
		{printf("\n\n\n");

			{
				printf("new loop\n");
				killed=0;io=0;
				deletefirstNode(&readyq);

				printf("after deletion \n");
				printList(readyq);
				running.pid = pid_pr;
				pid  = pid_pr;
				printf("run pid %d \n",pid );
				if(aa[pid].flag==0)
				{
					aa[pid].ftime=clock();
					aa[pid].flag=1;
				}

				kill(pid,SIGUSR2);
				
				start=clock();
				usleep(500);
				for(i=0;i<tmquanta;i++)
				{
					if(killed!=0 || io!=0)
					{
						break;
					}
				}
				end=clock();
				cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
				running.total_runtime=running.total_runtime + cpu_time_used;
				if(killed==0 && io==0)
				{
			
					printf(" %dTime quantum finished !!!\n",pid );
					kill(pid,SIGUSR1); //suspend
			
					tempnode.pid=running.pid;
					tempnode.priority=running.priority;
					tempnode.total_runtime=running.total_runtime;
					append(&readyq);
				}
			}

			check();
		}


		printf(" Done !\n");

	}
	

	// PR

	else
	{

		msgrcv(msqid,&msg,MAX,1,MSG_NOERROR);
		token=strtok(msg.data,s);
		pid=atoi(token);
		pid_pr = pid;
		token=strtok(NULL,s);
		priority=atoi(token);
		printf("In PR PID %d priority %d \n",pid,priority);
		tempnode.pid=pid;
		printf("temp: %d\n", tempnode.pid);
		tempnode.priority=priority;
		tempnode.total_runtime=0;
		aa[pid].flag=0;
		aa[pid].intime=clock();
		append(&readyq);

		sprintf(msg.data,"%d",getpid());
		msg.type=pid;
		msgsnd(msqid,&msg,MAX,0);

		killed=0;io=0;
		deletefirstNode(&readyq);
		running.pid = pid;
		if(aa[pid].flag==0)
		{
			aa[pid].ftime=clock();
			aa[pid].flag=1;
		}

		start=clock();
		usleep(500);
		kill(pid,SIGUSR2);
		for(i=0;i<tmquanta;i++)
		{
			if(killed!=0 || io!=0)
			{
				break;
			}
		}
		end=clock();
		cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
		running.total_runtime=running.total_runtime + cpu_time_used;
		if(killed==0 && io==0)
		{
		
			kill(pid,SIGUSR1);
			tempnode.pid=running.pid;
			tempnode.priority=running.priority;
			tempnode.total_runtime=running.total_runtime;
			append(&readyq);
		
		}
		
		sleep(1);
		for(i=0;i<3;i++)
		{
		
			msgrcv(msqid,&msg,MAX,1,MSG_NOERROR);
			token=strtok(msg.data,s);
			pid=atoi(token);
			token=strtok(NULL,s);
			priority=atoi(token);
			printf("In PR PID %d Priority %d \n",pid,priority);
			tempnode.pid=pid;
			tempnode.priority=priority;
			tempnode.total_runtime=0;
			aa[pid].flag=0;
			aa[pid].intime=clock();
			append(&readyq);
		
			sprintf(msg.data,"%d",getpid());
			msg.type=pid;
			msgsnd(msqid,&msg,MAX,0);
		}

		sleep(5);
int kao=10;
		while(termination!=4)  
		{
			kao--;
			if(kao==0)
				exit(0);
			printf("\n\n\n");
			if(readyq!=NULL)
			{


				printf("new loop\n");
				killed=0;io=0;

				pid=getmin(readyq);
			
				printf("max priority pid is %d\n",pid );
				deleteNode(&readyq,pid);
				

				printf("after deletion \n");
				printList(readyq);
				pid=running.pid;
				if(aa[pid].flag==0)
				{
					aa[pid].ftime=clock();
					aa[pid].flag=1;
				}
			
				start=clock();
				usleep(500);
				kill(pid,SIGUSR2);

				printf("run pid %d \n",pid );

				for(i=0;i<tmquanta;i++)
				{
					if(killed!=0 || io!=0)
					{
						break;
					}
				}
				end=clock();
				cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
				running.total_runtime=running.total_runtime + cpu_time_used;
				if(killed==0 && io==0)
				{
				
					printf(" %d your time quata has expired\n",pid );
					kill(pid,SIGUSR1);

					tempnode.pid=running.pid;
					tempnode.priority=running.priority;
					tempnode.total_runtime=running.total_runtime;
					append(&readyq);
				}
			}
			


			check();
		}
		
	}


	FILE *fp=fopen("result.txt","a");
	fprintf(fp,"average values of response time , waiting time and turnaround time");
	fprintf(fp,"\t\t%f\t%f\t%f\n",a1/4.0,b1/4.0,c1/4.0);
	fclose(fp); 

	return 0;
}

void append(struct node** head_ref)
{

    struct node* new_node = (struct node*) malloc(sizeof(struct node));
 
    struct node *last = *head_ref;  
  

    new_node->pid  = tempnode.pid;
    new_node->priority = tempnode.priority;
    new_node->total_runtime=tempnode.total_runtime;


    new_node->next = NULL;

    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
      
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;    
}


void deletefirstNode(struct node **head_ref)
{
	int position=0;

   if (*head_ref == NULL)
      return;
 
   struct node* temp = *head_ref;

   if(io==0)
   	running=*temp;
 

    if (position == 0)
    {
        *head_ref = temp->next; 
        free(temp);              
        return;
    }

    int i;
    for (i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;
 

    if (temp == NULL || temp->next == NULL)
         return;
 
 
    struct node *next = temp->next->next;
 

    free(temp->next);  // Free memory
 
    temp->next = next; 
}

void deleteNode(struct node **head_ref, int key)
{

    struct node* temp = *head_ref, *prev;
 	

    if (temp != NULL && temp->pid == key)
    {
    	if(flg==2)
    	{
    		running2=*temp;
    	}
    	else
    		running=*temp;
        *head_ref = temp->next;  
        free(temp);              
        return;
    }
 
  
    while (temp != NULL && temp->pid != key)
    {
        prev = temp;
        temp = temp->next;
    }
 

    if (temp == NULL) return;

    if(flg==2)
    	running2=*temp;
    else	
    	running=*temp;
 
    prev->next = temp->next;
 
    free(temp);  // Free memory
}


int getmin(struct node *nod)
{
	int flag=0,min,pid;
	while (nod != NULL)
	{
		if(flag==0)
		{
			min=nod->priority;
			pid=nod->pid;
			flag=1;
		}
		if(nod->priority < min)
		{
			min=nod->priority;
			pid=nod->pid;
		}
		nod = nod->next;

	}
	return pid;
}



void iorequest()
{
	io=1;

	printf("iorequest raised..add to waitingq\n");
	

	int pid=running.pid;

	end=clock();
	cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
	running.total_runtime=running.total_runtime + cpu_time_used;

	tempnode.pid=running.pid;
	tempnode.priority=running.priority;
	tempnode.total_runtime=running.total_runtime;

	append(&waitingq);	

}

void terminate()
{
	printf("termination of %d \n",running.pid );

	termination++;
	killed=1;
	cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;
	running.total_runtime=running.total_runtime + cpu_time_used;
	int pid=pid_pr;


	aa[pid].outtime=clock();	
	a=((double)(aa[pid].ftime - aa[pid].intime ))/CLOCKS_PER_SEC;
	c=((double)(aa[pid].outtime - aa[pid].intime ))/CLOCKS_PER_SEC;
	b=(c - running.total_runtime );
	a1=a1+a;
	b1=b1+b;
	c1=c1+c;
	FILE *fp=fopen("result.txt","a");
	fprintf(fp,"Process PID\t\tResponseT\tWaitingT\tTurnaroundT\t(in seconds)\n");
	fprintf(fp,"%d\t\t\t%f\t%f\t%f\n",pid_pr,a,b,c);
	fclose(fp); 
}

void check()
{
	printf("checking for any awaking processses\n");
	int status=msgrcv(msqid,&msg,MAX,2,IPC_NOWAIT);
	if(status==-1)
	{

		return;
	}
	int pi=atoi(msg.data);
	printf("process with pid %d is waking \n",pi );

	flg=2;
	deleteNode(&waitingq,pi);

	tempnode.pid=running2.pid;
	tempnode.priority=running2.priority;
	tempnode.total_runtime=running2.total_runtime;
	flg=0;
	append(&readyq);
	return;

}

