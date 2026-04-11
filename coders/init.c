#include "codexion.h"

int is_complet_compile(t_coder *coder)
{
    return get_compile_count(coder) == coder->table->must_compile;
}

void compile(t_coder *coder)
{
    long now;
    if (coder->table->scheduler == FIFO) {
        if (!take_dongles_fifo(coder))
        {
            
            return;
        }
    } else {
        if (!take_dongles_edf(coder))
            return;
    }
    if (get_stop(coder->table))
        return;
    pthread_mutex_lock(&coder->table->print_lock);
    now = gettimenow();
    // coder->last_compile = now;
    set_last_compile(coder, now);
    smart_sleep(coder->table->dongle_cooldown, coder->table);
    printf("%ld %d is compiling\n", now - coder->table->start_time, coder->id);
    pthread_mutex_unlock(&coder->table->print_lock);
    smart_sleep(coder->table->time_to_compile, coder->table);
    // coder->compile_count++; 
    increment_compile_count(coder);
    if (coder->table->scheduler == FIFO)
        release_dongles_fifo(coder);
    else
        release_dongles_edf(coder);
}

void debugging(t_coder *coder)
{
    if (get_stop(coder->table))
        return;
    pthread_mutex_lock(&coder->table->print_lock);
    printf("%ld %d is debugging\n", gettimenow() - coder->table->start_time, coder->id);
    pthread_mutex_unlock(&coder->table->print_lock);
    // usleep(coder->table->time_to_debug * 1000);
    smart_sleep(coder->table->time_to_debug, coder->table);
}

void refactoring(t_coder *coder)
{
    if (get_stop(coder->table))
        return;
    pthread_mutex_lock(&coder->table->print_lock);
    printf("%ld %d is refactoring\n", gettimenow() - coder->table->start_time, coder->id);
    pthread_mutex_unlock(&coder->table->print_lock);
    // usleep(coder->table->time_to_refactor * 1000);
    smart_sleep(coder->table->time_to_refactor, coder->table);
}

long gettimenow()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int take_dongles(t_coder *coder)
{
    if (get_stop(coder->table))
        return 0;
    pthread_mutex_t *first;
    pthread_mutex_t *second;
    if (coder->left < coder->right)
    {
        first = coder->left;
        second = coder->right;
    }
    else
    {
        first = coder->right;
        second = coder->left;
    }
    pthread_mutex_lock(first);
    pthread_mutex_lock(second);
    pthread_mutex_lock(&coder->table->print_lock);
    if (get_stop(coder->table))
    {
        pthread_mutex_unlock(first);
        pthread_mutex_unlock(second);
        // pthread_mutex_unlock(&coder->table->print_lock);
        return 0;
    }
    printf("%ld %d has taken a dongle\n", gettimenow() - coder->table->start_time, coder->id);
    pthread_mutex_unlock(&coder->table->print_lock);
    return 1;
}

void give_dongles(t_coder *coder)
{
    pthread_mutex_unlock(coder->left);
    pthread_mutex_unlock(coder->right);
}

int coder_is_burned(t_coder *coder)
{
    long now = gettimenow();
    return now - coder->last_compile > coder->table->time_to_burnout;
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    while (!get_stop(coder->table) && !is_complet_compile(coder))
    {
        if (get_stop(coder->table) || is_complet_compile(coder))
            break;
        compile(coder);
        if (get_stop(coder->table) || is_complet_compile(coder))
            break;
        debugging(coder);
        if (get_stop(coder->table) || is_complet_compile(coder))
            break;
        refactoring(coder); 
    }
    
    return NULL;
}

void init_coder(t_coder *coder,t_table *table, int id)
{
    coder->table = table;
    coder->id = id + 1;
    coder->left = &table->dongles[id];
    coder->right = &table->dongles[(id + 1) % table->nb_coders];
    coder->compile_count = 0;
    coder->last_compile = table->start_time;
    pthread_create(&coder->thread, NULL, coder_routine, coder);
}

void init_coders(t_table *table)
{
    int counter = 0;
    while (counter < table->nb_coders)
    {
        init_coder(&table->coders[counter], table, counter);
        counter++;
    }
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
    pthread_mutex_init(&table->queue_lock, NULL);
    pthread_cond_init(&table->cond, NULL);
}


void init_mutex_coder(t_table *table)
{
    int i = 0;
    while (i < table->nb_coders)
    {
        pthread_mutex_init(&table->coders[i].compile_count_lock, NULL);
        pthread_mutex_init(&table->coders[i].last_compile_lock, NULL);
        i++;
    }
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
    table->heap.size = 0;
    table->dongles = malloc(sizeof(pthread_mutex_t) * table->nb_coders);
    table->coders = malloc(sizeof(t_coder) * table->nb_coders);
    table->start_time = gettimenow();
    init_queue(&table->queue);
    init_mutex(table);
    init_mutex_coder(table);
    init_coders(table);
    return table;
}