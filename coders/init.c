#include "codexion.h"

void compile(t_coder *coder)
{
    printf("hello %d\n", coder->id);
    usleep(3000000);
}

long gettimenow()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    
    
}

void take_dongles(t_coder *coder)
{
    pthread_mutex_lock(coder->left);
    pthread_mutex_lock(coder->right);
    // printf("")
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    compile(coder);
    return NULL;
}

void init_coder(t_coder *coder,t_table *table, int id)
{
    coder->table = table;
    coder->id = id + 1;
    coder->left = &table->dongles[id];
    coder->right = &table->dongles[(id + 1) % (table->nb_coders - 1)];
    pthread_create(&coder->thread, NULL, coder_routine, coder);
    pthread_join(coder->thread, NULL);
}

void init_coders(t_table *table)
{
    t_coder *coders;
    int counter = 0;
    coders = malloc(sizeof(t_coder) * table->nb_coders);
    while (counter < table->nb_coders)
    {
        init_coder(&coders[counter], table, counter);
        counter++;
    }
    table->coders = coders;
}


int *parser(char **av, int ac)
{
    int i = 1;
    int *param;
    param = malloc(sizeof(int) * 7);
    while (i < ac - 1)
    {
        param[i - 1] = ft_atoi(av[i]);
        i++;
    }
    return param;
}

void init_mutex(t_table *table)
{
    int i = 0;
    while (i < table->nb_coders)
        pthread_mutex_init(&table->dongles[i++], NULL);
    pthread_mutex_init(&table->print_lock, NULL);
    pthread_mutex_init(&table->stop_lock, NULL);
}

t_table *init_parameter(int *paramter, char *scheduler)
{
    t_table *table;
    t_scheduler sch = FIFO;
    if (ft_strcmp_adv(scheduler, "edf"))
        sch = EDF;
    table = malloc(sizeof(t_table));
    table->nb_coders = paramter[0];
    table->time_to_burnout = paramter[1];
    table->time_to_compile = paramter[2];
    table->time_to_debug = paramter[3];
    table->time_to_refactor = paramter[4];
    table->must_compile = paramter[5];
    table->dongle_cooldown = paramter[6];
    table->scheduler = sch;
    table->stop = 0;
    table->dongles = malloc(sizeof(pthread_mutex_t) * table->nb_coders);
    init_mutex(table);
    init_coders(table);
    return table;
}