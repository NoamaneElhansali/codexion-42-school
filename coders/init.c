/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:54 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 20:35:38 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle(t_dongle *dongle, t_table *t)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->last_used = 0;
	if (t->scheduler == FIFO)
	{
		dongle->queue.data = malloc(sizeof(int) * 2);
	}
	else
	{
		dongle->heap.data = malloc(sizeof(t_coder *) * 2);
	}
	dongle->queue.f = 0;
	dongle->queue.r = 0;
	dongle->heap.size = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	init_dongles(t_table *table)
{
	int	counter;

	counter = 0;
	while (counter < table->nb_coders)
		if (!init_dongle(&table->dongles[counter++], table))
			return (0);
	return (1);
}

int	init_coders(t_table *table)
{
	int	counter;

	counter = 0;
	while (counter < table->nb_coders)
	{
		if (!init_coder(&table->coders[counter], table, counter))
			return (0);
		counter++;
	}
	return (1);
}

int	init_mutex(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_coders)
	{
		if ((pthread_mutex_init(&table->dongles[i].mutex, NULL) != 0)
			|| (pthread_mutex_init(&table->dongles[i].last_used_lock,
					NULL) != 0)
			|| (pthread_mutex_init(&table->dongles[i].queue_lock, NULL) != 0)
			|| (pthread_cond_init(&table->dongles[i].cond, NULL) != 0))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&table->print_lock, NULL) != 0
		|| pthread_mutex_init(&table->stop_lock, NULL) != 0
		|| pthread_mutex_init(&table->complite_sim_lock, NULL) != 0)
		return (0);
	return (1);
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
	table->dongles = malloc(sizeof(t_dongle) * table->nb_coders);
	table->coders = malloc(sizeof(t_coder) * table->nb_coders);
	table->start_time = gettimenow();
	init_mutex(table);
	init_mutex_coder(table);
	init_dongles(table);
	init_coders(table);
	return (table);
}
