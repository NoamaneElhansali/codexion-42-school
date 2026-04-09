#ifndef CODEXION_H
# define CODEXION_H
# define INT_MAX 2147483647
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_coder
{
    int id;
    pthread_t thread;

    long last_compile;
    int compile_count;

    pthread_mutex_t *left;
    pthread_mutex_t *right;

    struct s_table *table;
} t_coder;

typedef enum {
    FIFO,
    EDF
} t_scheduler;

typedef struct s_queue
{
    int data[200];
    int f;
    int r;
} t_queue;

// typedef struct s_request
// {
//     int id;
//     long deadline
// } t_request;

typedef struct s_heap
{
    t_coder *data[200];
    int size;
} t_heap;



typedef struct s_table
{
    int nb_coders;
    long            time_to_burnout;
    long            time_to_compile;
    long            time_to_debug;
    long            time_to_refactor;
    long            start_time;
    int must_compile;
    long dongle_cooldown;
    int stop;
    pthread_mutex_t *dongles;
    pthread_mutex_t print_lock;
    pthread_mutex_t stop_lock;
    pthread_mutex_t queue_lock;
    pthread_cond_t cond;
    t_scheduler scheduler;
    pthread_t thread_monitor;
    t_queue queue;
    t_heap heap;
    t_coder *coders;

} t_table;







int parser_parameter(char **av, int ac);
long	ft_atoi(const char *nptr);
int	ft_strcmp_adv(const char *s1, const char *s2);
void compile(t_coder *coder);
void *coder_routine(void *arg);
void init_coder(t_coder *coder,t_table *table, int id);
void init_coders(t_table *table);
int *parser(char **av, int ac);
void init_mutex(t_table *table);
t_table *init_parameter(int *paramter, char *scheduler);
long gettimenow();
int get_stop(t_table *table);
void set_stop(t_table *table);
void start_monitoring(t_table *table);
void *monitor(void *arg);
void debugging(t_coder *coder);
void refactoring(t_coder *coder);
void take_dongles(t_coder *coder);
void give_dongles(t_coder *coder);
void join_coders(t_table *table);
void push(t_queue *q, int id);
int pop(t_queue *q);
void init_queue(t_queue *q);
int take_dongles_fifo(t_coder *coder);
void release_dongles_fifo(t_coder *coder);
int coder_is_burned(t_coder *coder);
long get_deadline(t_coder *coder);
void add_to_heap(t_heap *heap, t_coder *coder);
void remove_min(t_heap *heap);
t_coder *get_min_heap(t_heap *heap);
int take_dongles_edf(t_coder *coder);
void release_dongles_edf(t_coder *coder);
int is_in_first_fifo(t_queue *q, int id, int n);
int is_in_first_edf(t_heap *h, int id, int n);
void smart_sleep(long time, t_table *t);

# endif