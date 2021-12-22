 #define MAX 20
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *monitor_get_hair_cut(void *param);
void *monitor_cut_hair(void *param);

struct monitor_data{
int TotalChairs; //global total seats
sem_t chairs_mutex;
sem_t sem_customer;
sem_t sem_barber;
int VacantChairs; //input -> keep editing
int customerWait;
};
struct monitor_data monitor;


int main() {
	pthread_t barberid;
	pthread_t customerids[MAX];
	
	printf("Shop opens\n");
   int runTime,numOfCustomers,i;

 printf("Enter total number of Customers : \n");
   scanf("%d",&numOfCustomers);
   printf("Enter number of chairs : \n");
   scanf("%d", &(monitor.VacantChairs));

monitor.TotalChairs = monitor.VacantChairs;

   runTime = 5;
 
   monitor.customerWait = 1;

   sem_init(&(monitor.chairs_mutex),0,1);
   sem_init(&(monitor.sem_customer),0,0);
   sem_init(&(monitor.sem_barber),0,0);
 
   pthread_create(&barberid, NULL, monitor_cut_hair, NULL);
   printf("Barber is ready to serve\n");
 
   for (i = 0; i < numOfCustomers; i++){
      printf("Creating customer thread : id  = %d\n",i+1);
	   pthread_create(&customerids[i], NULL, monitor_get_hair_cut, NULL);
	   
   }
//end
   printf("End\n");
   exit(0);
}

void *monitor_cut_hair(void *param) {
   int worktime;
  
   while(1) {
    
	  sem_wait(&(monitor.sem_customer));
  
	  sem_wait(&(monitor.chairs_mutex));
  
	  monitor.VacantChairs += 1;
	  printf("Barber busy.... Number of chairs available = %d\n",monitor.VacantChairs);
   if(monitor.VacantChairs==monitor.TotalChairs){
      printf("No waiting customer . Barber sleeps ......\n");
   }
	  sem_post(&(monitor.sem_barber)); //barber get ready
printf("Barber is ready to cut hair\n");
	  sem_post(&(monitor.chairs_mutex));
    
	  printf("Barber is cutting hair...\n");
	  sleep(3);
    } 
}

void *monitor_get_hair_cut(void *param) {
   int waittime;

   while(1) {
     
	  sem_wait(&(monitor.chairs_mutex));
  
	  if(monitor.VacantChairs <= 0){
        
		   printf("Customer %u leaving with no haircut\n", (unsigned int)pthread_self()-1); //no chair avail => cust leaves
		   sem_post(&(monitor.chairs_mutex));
	  }
	  else{ //seat avail
         
		   monitor.VacantChairs -= 1; //given to new cust
		   printf("Customer %u is waiting on seat. Number of seats left = %d\n",(unsigned int)pthread_self()-1,monitor.VacantChairs);
 
		   sem_post(&(monitor.sem_customer));
		   sem_post(&(monitor.chairs_mutex));
		   sem_wait(&(monitor.sem_barber)); //wait for barber to get ready
		   printf("Customer %u is getting a haircut\n",(unsigned int)pthread_self()-1);
	  }
   
	 waittime = (rand() % monitor.customerWait) + 1;
	  sleep(waittime);
     }
}