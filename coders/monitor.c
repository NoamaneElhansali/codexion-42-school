/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:01 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 22:06:53 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_monitoring(t_table *table)
{
	pthread_create(&table->thread_monitor, NULL, monitor, table);
	pthread_join(table->thread_monitor, NULL);
	join_coders(table);
}

void	join_coders(t_table *table)
{
	int	counter;

	counter = 0;
	while (counter < table->nb_coders)
	{
		pthread_join(table->coders[counter].thread, NULL);
		counter++;
	}
}

int	check_must_complie(t_table *table)
{
	int	all_done;

	pthread_mutex_lock(&table->complite_sim_lock);
	all_done = (table->complite_sim == table->nb_coders);
	pthread_mutex_unlock(&table->complite_sim_lock);
	return (all_done);
}

int	check_similation(t_table *table, long now, int i)
{
	if (get_last_compile(&table->coders[i]) + table->time_to_burnout <= now)
	{
		set_stop(table);
		broadcast_coder(table);
		pthread_mutex_lock(&table->print_lock);
		printf("\033[31m%ld %d burned out\033[0m\n", now - table->start_time,
			table->coders[i].id);
		pthread_mutex_unlock(&table->print_lock);
		return (1);
	}
	if (check_must_complie(table))
	{
		set_stop(table);
		broadcast_coder(table);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_table	*table;
	int		i;
	long	now;

	table = (t_table *)arg;
	while (!get_stop(table))
	{
		i = 0;
		while (i < table->nb_coders)
		{
			now = gettimenow();
			if (check_similation(table, now, i))
				return (NULL);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
