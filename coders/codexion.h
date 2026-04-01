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

typedef struct s_table
{
    int nb_coders;
    long            time_to_burnout;
    long            time_to_compile;
    long            time_to_debug;
    long            time_to_refactor;
    int must_compile;
    long dongle_cooldown;
    int stop;
    pthread_mutex_t *dongles;
    pthread_mutex_t print_lock;
    pthread_mutex_t stop_lock;
    t_scheduler scheduler;
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

# endif