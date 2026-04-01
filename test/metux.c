#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
void *work(void *arg)
{
    for (size_t i = 0; i < 500; i++)
        counter++;
    return NULL;
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_mutex_t f1;

    pthread_create(&t1, NULL, work, NULL);
    pthread_create(&t2, NULL, work, NULL);
    pthread_create(&t3, NULL, work, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("%d", counter);
    return 0;
}