/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:54 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 02:36:12 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->last_used = gettimenow();
	pthread_mutex_unlock(&dongle->mutex);
}

void	init_coder(t_coder *coder, t_table *table, int id)
{
	coder->table = table;
	coder->id = id + 1;
	coder->left = &table->dongles[id];
	coder->right = &table->dongles[(id + 1) % table->nb_coders];
	coder->compile_count = 0;
	coder->last_compile = table->start_time;
	pthread_create(&coder->thread, NULL, coder_routine, coder);
}

void	init_dongles(t_table *table)
{
	int	counter;

	counter = 0;
	while (counter < table->nb_coders)
		init_dongle(&table->dongles[counter++]);
}

void	init_coders(t_table *table)
{
	int	counter;

	counter = 0;
	while (counter < table->nb_coders)
	{
		init_coder(&table->coders[counter], table, counter);
		counter++;
	}
}

void	init_mutex(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_coders)
	{
		pthread_mutex_init(&table->dongles[i].mutex, NULL);
		pthread_mutex_init(&table->dongles[i].last_used_lock, NULL);
		i++;
	}
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->stop_lock, NULL);
	pthread_mutex_init(&table->queue_lock, NULL);
	pthread_mutex_init(&table->complite_sim_lock, NULL);
	pthread_cond_init(&table->cond, NULL);
}

void	init_mutex_coder(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_coders)
	{
		pthread_mutex_init(&table->coders[i].compile_count_lock, NULL);
		pthread_mutex_init(&table->coders[i].last_compile_lock, NULL);
		i++;
	}
}

t_table	*init_parameter(int *paramter, char *scheduler)
{
	t_table		*table;
	t_scheduler	sch;

	sch = FIFO;
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
	table->complite_sim = 0;
	table->heap.size = 0;
	table->heap.data = malloc(sizeof(t_coder) * table->nb_coders);
	table->queue.data = malloc(sizeof(int) * table->nb_coders);
	table->dongles = malloc(sizeof(t_dongle) * table->nb_coders);
	table->coders = malloc(sizeof(t_coder) * table->nb_coders);
	table->start_time = gettimenow();
	init_queue(&table->queue);
	init_mutex(table);
	init_dongles(table);
	init_coders(table);
	return (table);
}
