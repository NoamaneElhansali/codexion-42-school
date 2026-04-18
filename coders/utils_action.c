/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 20:01:36 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/18 21:29:00 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_last_use(t_dongle *d, long now)
{
	pthread_mutex_lock(&d->last_used_lock);
	d->last_used = now;
	pthread_mutex_unlock(&d->last_used_lock);
}

void	remove_in_queue(t_dongle *d)
{
	pthread_mutex_lock(&d->queue_lock);
	pop(&d->queue);
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->queue_lock);
}

void	remove_in_heap(t_dongle *d)
{
	pthread_mutex_lock(&d->queue_lock);
	remove_min(&d->heap);
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->queue_lock);
}

void	give_dongles(t_coder *coder)
{
	long	now;

	now = gettimenow();
	if (coder->table->scheduler == FIFO)
	{
		remove_in_queue(coder->left);
		remove_in_queue(coder->right);
	}
	else
	{
		remove_in_heap(coder->left);
		remove_in_heap(coder->right);
	}
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
	set_last_use(coder->left, now);
	set_last_use(coder->right, now);
}

int	init_coder(t_coder *coder, t_table *table, int id)
{
	coder->table = table;
	coder->id = id + 1;
	coder->left = &table->dongles[id];
	coder->right = &table->dongles[(id + 1) % table->nb_coders];
	coder->compile_count = 0;
	coder->last_compile = table->start_time;
	if (pthread_create(&coder->thread, NULL, coder_routine, coder) != 0)
		return (0);
	return (1);
}
