/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:51 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 17:15:42 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	push(t_queue *q, int id)
{
	q->data[q->r++ % 2] = id;
}

int	pop(t_queue *q)
{
	return (q->data[q->f++ % 2]);
}

void wait_queue(t_dongle *d, t_coder *coder)
{
	pthread_mutex_lock(&d->queue_lock);
	push(&d->queue, coder->id);
	while ((d->queue.data[d->queue.f % 2] != coder->id) && !get_stop(coder->table))
		pthread_cond_wait(&d->cond, &d->queue_lock);
	pthread_mutex_unlock(&d->queue_lock);
}

int	take_dongles_fifo(t_coder *coder)
{
	t_table	*t;

	t = coder->table;

	if (get_stop(t))
		return (0);
	if (!take_dongles(coder))
		return (0);
	return (1);
}

void	release_dongles_fifo(t_coder *coder)
{
	give_dongles(coder);
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
