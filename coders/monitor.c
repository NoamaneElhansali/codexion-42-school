#include "codexion.h"
int get_stop(t_table *table)
{
    int value;

    pthread_mutex_lock(&table->stop_lock);
    value = table->stop;
    pthread_mutex_unlock(&table->stop_lock);
    return (value);
}

void set_stop(t_table *table)
{
    pthread_mutex_lock(&table->stop_lock);
    table->stop = 1;
    pthread_mutex_unlock(&table->stop_lock);
}

void start_monitoring(t_table *table)
{
    pthread_create(&table->thread_monitor, NULL, monitor, table);
    pthread_join(table->thread_monitor, NULL);
    join_coders(table);
}

void join_coders(t_table *table)
{
    int counter = 0;
    while (counter < table->nb_coders)
    {
        pthread_join(table->coders[counter].thread, NULL);
        counter++;
    }
}


int check_must_complie(t_table *table)
{
    int all_done = 1;

    int j = 0;
    while (j < table->nb_coders)
    {
        if (get_compile_count(&table->coders[j]) < table->must_compile)
            all_done = 0;
        j++;
    }
    return all_done;
}

void *monitor(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    long now;

    while (!get_stop(table))
    {
        i = 0;
        while (i < table->nb_coders)
        {
            now = gettimenow();
            if (get_last_compile(&table->coders[i]) + table->time_to_burnout < now)
            {
                set_stop(table);
                pthread_mutex_lock(&table->queue_lock);
                pthread_cond_broadcast(&table->cond);
                pthread_mutex_unlock(&table->queue_lock);
                pthread_mutex_lock(&table->print_lock);
                printf("%ld %d burned out\n", now - table->start_time, table->coders[i].id);
                pthread_mutex_unlock(&table->print_lock);
                return (NULL);
            }
            if (check_must_complie(table))
            {
                set_stop(table);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
}