#include "codexion.h"

long get_last_compile(t_coder *coder)
{
    long value;
    pthread_mutex_lock(&coder->last_compile_lock);
    value = coder->last_compile;
    pthread_mutex_unlock(&coder->last_compile_lock);
    return value;
}

void set_last_compile(t_coder *coder, long time)
{
    pthread_mutex_lock(&coder->last_compile_lock);
    coder->last_compile = time;
    pthread_mutex_unlock(&coder->last_compile_lock);
}

int get_compile_count(t_coder *coder)
{
    int i;
    pthread_mutex_lock(&coder->compile_count_lock);
    i = coder->compile_count;
    pthread_mutex_unlock(&coder->compile_count_lock);
    return i;
}

void increment_compile_count(t_coder *coder)
{
    pthread_mutex_lock(&coder->compile_count_lock);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->compile_count_lock);
}