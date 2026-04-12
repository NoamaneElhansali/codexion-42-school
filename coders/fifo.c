/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:51 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 02:26:28 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	push(t_queue *q, int id)
{
	q->data[q->r++] = id;
}

int	pop(t_queue *q)
{
	return (q->data[q->f++]);
}

int	take_dongles_fifo(t_coder *coder)
{
	t_table	*t;

	t = coder->table;
	pthread_mutex_lock(&t->queue_lock);
	push(&t->queue, coder->id);
	while (!(t->queue.data[t->queue.f] != coder->id) && !get_stop(t))
		pthread_cond_wait(&t->cond, &t->queue_lock);
	pthread_mutex_unlock(&t->queue_lock);
	if (get_stop(t))
		return (0);
	if (!take_dongles(coder))
		return (0);
	return (1);
}

void	release_dongles_fifo(t_coder *coder)
{
	give_dongles(coder);
	pthread_mutex_lock(&coder->table->queue_lock);
	pop(&coder->table->queue);
	pthread_cond_broadcast(&coder->table->cond);
	pthread_mutex_unlock(&coder->table->queue_lock);
}

int	is_in_first_fifo(t_queue *q, int id, int n)
{
	int	i;
	int	count;

	i = q->f;
	count = 0;
	while (i < q->r && count < n)
	{
		if (q->data[i] == id)
			return (1);
		count++;
		i++;
	}
	return (0);
}
