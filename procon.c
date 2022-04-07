#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREAD 2
#define BUFFER_SIZE 100

sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void *producer(void *pno)
{
    int random;
    while (1)
    {
        random = rand();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = random;
        printf("producer: %d 생산 버퍼 %d번 자리\n", buffer[in], in);
        in %= BUFFER_SIZE;
        in++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        usleep(1500000);
    }
}

void *consumer(void *cno)
{
    while (1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("consumer: %d 소비, 버퍼 %d번 자리\n", item, out);
        out %= BUFFER_SIZE;
        out++;
        int n = buffer[out];
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        usleep(2000000);
    }
}

int main()
{
    printf("hello world\n");
    int num = 0;
    int rc = 0;
    int i = 0;
    int result[NUM_THREAD];
    pthread_t thread[NUM_THREAD];
    pthread_create(&thread[0], NULL, &producer, (void *)&num);
    pthread_create(&thread[1], NULL, &consumer, (void *)&num);

    for (i = 0; i < NUM_THREAD; i++)
    {
        rc = pthread_join(thread[i], (void **)&result[i]);
        if (rc != 0)
        {
            printf("Error in thread[%d] : %d\n", i, rc);
            exit(1);
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
