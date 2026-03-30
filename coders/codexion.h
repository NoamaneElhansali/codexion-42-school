#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <pthread.h>

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
    t_coder *coders;

} t_table;



int parser_parameter(char **av, int ac);
int	ft_atoi(const char *nptr);
int	ft_strcmp(const char *s1, const char *s2);

# endif