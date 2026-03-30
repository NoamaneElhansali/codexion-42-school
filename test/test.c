#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *hello(void *arg)
{
    sleep(1);
    printf("Thread finiched\n");
    return NULL;
}

int main()
{
    pthread_t th;
    // char *result;
    // ;
    // pthread_join(th, (void **)&result);
    pthread_create(&th, NULL, hello, NULL);
    printf("%lg", th);
    pthread_join(th, NULL);
    printf("Main finished\n");
    return 0;
}
