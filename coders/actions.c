/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:38 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 16:27:18 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (get_stop(coder->table))
		return (0);
	first = coder->right;
	second = coder->left;
	if (coder->id % 2 == 0)
	{
		first = coder->left;
		second = coder->right;
	}
	if (!lock_dongles(first, coder))
		return (0);
    if (!lock_dongles(second, coder))
    {
        pthread_mutex_unlock(&first->mutex); 
        return (0);
    }
	return (1);
}

void set_last_use(t_dongle *d, long now)
{
	pthread_mutex_lock(&d->last_used_lock);
	d->last_used = now;
	pthread_mutex_unlock(&d->last_used_lock);
}

void remove_in_queue(t_dongle *d)
{
	pthread_mutex_lock(&d->queue_lock);
	pop(&d->queue);
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->queue_lock);
}

void remove_in_heap(t_dongle *d)
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
	}else
	{
		remove_in_heap(coder->left);
		remove_in_heap(coder->right);
	}
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
	set_last_use(coder->left, now);
	set_last_use(coder->right, now);
}

int	coder_is_burned(t_coder *coder)
{
	long	now;

	now = gettimenow();
	return (now - coder->last_compile > coder->table->time_to_burnout);
}

void	broadcast_coder(t_table *table)
{
	int i;

	i = 0;
	while (i < table->nb_coders)
	{	
		pthread_mutex_lock(&table->dongles[i].queue_lock);
		pthread_cond_broadcast(&table->dongles[i].cond);
		pthread_mutex_unlock(&table->dongles[i].queue_lock);
		i++;
	}
	
}

void	wait_cooldown(t_dongle *d, t_table *t)
{
	long	now;
	long	last;
	long	remaining;

	while (!get_stop(t))
	{
		now = gettimenow();
		pthread_mutex_lock(&d->last_used_lock);
		last = d->last_used;
		pthread_mutex_unlock(&d->last_used_lock);
		remaining = t->dongle_cooldown - (now - last);
		if (remaining <= 0)
			return ;
		usleep(100);
	}
}

int	lock_dongles(t_dongle *d, t_coder *coder)
{
	if (coder->table->scheduler == FIFO)
		wait_queue(d,coder);
	else
		wait_heap(d,coder);
	wait_cooldown(d, coder->table);
	pthread_mutex_lock(&d->mutex);
	if (get_stop(coder->table))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->table->print_lock);
	// printf("%ld\n", coder->table->time_to_burnout);
	printf("\033[32m%ld %d has taken a dongle\033[0m\n", gettimenow()
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	return (1);
}