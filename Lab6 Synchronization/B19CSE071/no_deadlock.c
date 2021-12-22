#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
sem_t numphil;
sem_t forks[5];

void eat(int x)
{
    printf("Philosopher %d is eating\n", x);
    fflush(stdout);
}

void *phil(void *num)
{
    int philo = *(int *)num;

    printf("Philosopher %d is eating\n", philo);
    fflush(stdout);
    sem_wait(&forks[philo]);
    sem_wait(&forks[(philo + 1) % 5]);
    eat(philo);
    sleep(2);
    printf("Philosopher %d has eaten\n", philo);
    fflush(stdout);
    sem_post(&forks[(philo + 1) % 5]);
    sem_post(&forks[philo]);
    sem_post(&numphil);
}

int main()
{
    int p[5];
    pthread_t threads[5];
    sem_init(&numphil, 0, 5);
    for (unsigned int i = 0; i < 5; ++i)
    {
        sem_init(&forks[i], 0, 1);
    }
    for (unsigned int i = 0; i < 5; ++i)
    {
        p[i] = i;
        srand(time(0));
        int random = rand() % (7);
        printf("Philosopher %d thinking out loud for %d seconds\n",i,random);
        // puts("Thinking out loud...");
        fflush(stdout);
        sleep(random);
        sem_wait(&numphil);
        pthread_create(&threads[i], NULL, phil, (void *)&p[i]);
    }
    for (unsigned int i = 0; i < 5; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}