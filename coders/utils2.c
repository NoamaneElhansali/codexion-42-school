/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:49:27 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 02:33:44 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongles_edf(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	give_dongles(coder);
	pthread_mutex_lock(&table->queue_lock);
	remove_min(&table->heap);
	pthread_cond_broadcast(&table->cond);
	pthread_mutex_unlock(&table->queue_lock);
}

int	is_in_first_edf(t_heap *h, int id, int n)
{
	int	i;

	i = 0;
	if (h->size == 0)
		return (0);
	while (i < h->size && i < n)
	{
		if (h->data[i]->id == id)
			return (1);
		i++;
	}
	return (0);
}

void	init_queue(t_queue *q)
{
	q->f = 0;
	q->r = 0;
}

int	get_stop(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_lock);
	value = table->stop;
	pthread_mutex_unlock(&table->stop_lock);
	return (value);
}

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
}
