/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:49:27 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/18 15:04:46 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongles_edf(t_coder *coder)
{
	give_dongles(coder);
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

t_coder	*get_min_heap(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->data[0]);
}

int	init_mutex_coder(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_coders)
	{
		if ((pthread_mutex_init(&table->coders[i].compile_count_lock,
					NULL) != 0)
			|| (pthread_mutex_init(&table->coders[i].last_compile_lock,
					NULL) != 0))
			return (0);
		i++;
	}
	return (1);
}
