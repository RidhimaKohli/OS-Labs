#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

sem_t num_phil;
sem_t forks[5];
int avail[5];
int req[5];   // to check the request list (if process 0 is requesting for fork 0, req[0]=0)
int eaten[5];
int total = 0;

bool deadlocked = 0;

void parray(int* a){
    for (int i=0; i<5; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void eat(int x){
    printf("Philosopher %d is eating\n", x);
    fflush(stdout);
}

void *phil(void *num){
    int philo = *(int *)num;

    sem_wait(&forks[philo]);
    req[philo] = philo; // booked the left fork
    sleep(10);
    sem_wait(&forks[(philo + 1) % 5]);
    // printf("Phil: %d picked the left fork and is waiting for right.\n", philo);

    // booked right fork
    // printf("Phil: %d picked the right fork\n", philo);
    req[(philo + 1) % 5] = philo;
    eat(philo);
    printf("Philosopher %d has eaten\n", philo);
    eaten[philo] = 1;
    fflush(stdout);
    req[philo] = -1;
    req[(philo + 1) % 5] = -1;
    total++;
    sem_post(&forks[(philo + 1) % 5]);
    sem_post(&forks[philo]);
    sem_post(&num_phil);
    
}


// to detect deadlock
bool check_lock(){
    printf("checking deadlocks\n");
    // parray(req);
    bool a=1;
    bool b=1;
    int first = req[0];
    // this look checks if every phil wants their left fork
    for (int i=0; i<5; i++){
        if (req[i]==-1){
            return false;
        }
        if (req[i]==(i+first)%5)
            ;
        else{
            a=false;
            break;
        }
    }

    // checks if every phil wants their right fork
    for (int i=0; i<5; i++){
        if (req[i]==(i-first)%5)
            ;
        else{
            b=false;
            break;
        }
    }
    // in either cases, deadlock is present
    if (a || b){
        printf("Deadlock detected\n");
        deadlocked = true;
        // exit(0);
        return true;
    }
    printf("No deadlock detected\n");
    return false;
}

int main(){
    int p[5];
    pthread_t threads[5];
    sem_init(&num_phil, 0, 5);

    for (int i=0; i<5; i++){
        req[i] = -1;
        eaten[i] = 0;
    }

    for (unsigned int i = 0; i < 5; ++i){
        sem_init(&forks[i], 0, 1);
    }

    bool locked = false;
    int vic;
    unsigned int i = 0;
    while (total < 5){
        if (eaten[i]==1){
            i = (i+1)%5;
            continue;
        }
        p[i] = i;
        srand(time(0));
        int random = rand() % (7);
        if (deadlocked && total==4){
             printf("Philosopher %d is eating\n", vic);
            printf("Philosopher %d has eaten\n", vic);
            exit(0);
        }
        else{
            int randomt = rand() % (7);
            printf("Philosopher %d thinking out loud for %d seconds\n",i,randomt);
        }

        fflush(stdout);
        sleep(1);
        sem_wait(&num_phil);
        pthread_create(&threads[i], NULL, phil, (void *)&p[i]);
        locked = check_lock(req);

        if (locked){
            vic = rand()%5;
            sem_post(&forks[vic]);
            pthread_cancel(threads[vic]);
            printf("Preempted %d\n", vic);
            sleep(10);
        }
        i = (i+1)%5;
    }
    for (unsigned int i = 0; i < 5; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}