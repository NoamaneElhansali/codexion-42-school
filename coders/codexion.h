/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:45 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 22:19:05 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define INT_MAX 2147483647
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_coder
{
	int				id;
	pthread_t		thread;

	long			last_compile;
	int				compile_count;
	pthread_mutex_t	last_compile_lock;
	pthread_mutex_t	compile_count_lock;

	struct s_dongle	*left;
	struct s_dongle	*right;

	struct s_table	*table;
}					t_coder;

typedef enum s_enum
{
	FIFO,
	EDF
}					t_scheduler;

typedef struct s_queue
{
	int				*data;
	int				f;
	int				r;
}					t_queue;

typedef struct s_heap
{
	t_coder			**data;
	int				size;
}					t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			last_used;
	pthread_mutex_t	last_used_lock;
	t_queue			queue;
	t_heap			heap;
	pthread_mutex_t	queue_lock;
	pthread_cond_t	cond;
}					t_dongle;

typedef struct s_table
{
	int				nb_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			start_time;
	int				must_compile;
	long			dongle_cooldown;
	int				stop;
	t_dongle		*dongles;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	complite_sim_lock;
	t_scheduler		scheduler;
	pthread_t		thread_monitor;
	int				complite_sim;
	t_coder			*coders;

}					t_table;

int					parser_parameter(char **av, int ac);
long				ft_atoi(const char *nptr);
int					ft_strcmp_adv(const char *s1, const char *s2);
int					compile(t_coder *coder);
void				*coder_routine(void *arg);
int					init_coder(t_coder *coder, t_table *table, int id);
int					init_coders(t_table *table);
int					*parser(char **av, int ac);
int					init_mutex(t_table *table);
t_table				*init_parameter(int *paramter, char *scheduler);
long				gettimenow(void);
int					get_stop(t_table *table);
void				set_stop(t_table *table);
void				start_monitoring(t_table *table);
void				*monitor(void *arg);
int					debugging(t_coder *coder);
int					refactoring(t_coder *coder);
int					take_dongles(t_coder *coder);
void				give_dongles(t_coder *coder);
void				join_coders(t_table *table);
void				push(t_queue *q, int id);
int					pop(t_queue *q);
int					take_dongles_fifo(t_coder *coder);
void				release_dongles_fifo(t_coder *coder);
long				get_deadline(t_coder *coder);
void				add_to_heap(t_heap *heap, t_coder *coder);
void				remove_min(t_heap *heap);
t_coder				*get_min_heap(t_heap *heap);
int					take_dongles_edf(t_coder *coder);
void				release_dongles_edf(t_coder *coder);
void				smart_sleep(long time, t_table *t);
void				init_mutex_coder(t_table *table);
long				get_last_compile(t_coder *coder);
void				set_last_compile(t_coder *coder, long time);
int					get_compile_count(t_coder *coder);
void				increment_compile_count(t_coder *coder);
int					is_complet_compile(t_coder *coder);
void				broadcast_coder(t_table *table);
int					check_similation(t_table *table, long now, int i);
int					lock_dongles(t_dongle *d, t_coder *coder);
void				wait_heap(t_dongle *d, t_coder *coder);
void				wait_queue(t_dongle *d, t_coder *coder);

#endif
