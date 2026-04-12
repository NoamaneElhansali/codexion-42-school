/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:48 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 01:22:23 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_deadline(t_coder *coder)
{
	long	last;

	pthread_mutex_lock(&coder->last_compile_lock);
	last = coder->last_compile;
	pthread_mutex_unlock(&coder->last_compile_lock);
	return (last + coder->table->time_to_burnout);
}

void	add_to_heap(t_heap *heap, t_coder *coder)
{
	int		i;
	t_coder	*tmp;
	int		p;

	i = heap->size;
	heap->data[i] = coder;
	heap->size++;
	while (i > 0)
	{
		p = (i - 1) / 2;
		if (get_deadline(heap->data[p]) <= get_deadline(heap->data[i]))
			break ;
		tmp = heap->data[p];
		heap->data[p] = heap->data[i];
		heap->data[i] = tmp;
		i = p;
	}
}

void	remove_min(t_heap *heap)
{
	t_coder	*tmp;

	int(i), (j), (z), (smallest);
	i = 0;
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	while (1)
	{
		j = i * 2 + 1;
		z = i * 2 + 2;
		smallest = i;
		if (j < heap->size
			&& get_deadline(heap->data[j]) < get_deadline(heap->data[smallest]))
			smallest = j;
		if (z < heap->size
			&& get_deadline(heap->data[z]) < get_deadline(heap->data[smallest]))
			smallest = z;
		if (smallest == i)
			break ;
		tmp = heap->data[i];
		heap->data[i] = heap->data[smallest];
		heap->data[smallest] = tmp;
		i = smallest;
	}
}

t_coder	*get_min_heap(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->data[0]);
}

int	take_dongles_edf(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->queue_lock);
	add_to_heap(&table->heap, coder);
	while (!(get_min_heap(&table->heap)->id != coder->id) && !get_stop(table))
		pthread_cond_wait(&table->cond, &table->queue_lock);
	pthread_mutex_unlock(&table->queue_lock);
	if (get_stop(table))
		return (0);
	if (!take_dongles(coder))
		return (0);
	return (1);
}
