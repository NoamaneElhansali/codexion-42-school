/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:07 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/13 19:50:46 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile_suit(t_coder *coder, long now)
{
	pthread_mutex_lock(&coder->table->print_lock);
	set_last_compile(coder, now);
	printf("\033[32m%ld %d is compiling\033[0m\n", now
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	increment_compile_count(coder);
	smart_sleep(coder->table->time_to_compile, coder->table);
	if (coder->table->scheduler == FIFO)
		release_dongles_fifo(coder);
	else
		release_dongles_edf(coder);
}

int	compile(t_coder *coder)
{
	long	now;

	if (coder->table->scheduler == FIFO)
	{
		if (!take_dongles_fifo(coder))
			return 0;
	}
	else
	{
		if (!take_dongles_edf(coder))
			return 0;
	}
	if (get_stop(coder->table))
	{
		give_dongles(coder);
		return 0;
	}
	now = gettimenow();
	compile_suit(coder, now);
	return 1;
}

int	debugging(t_coder *coder)
{
	if (get_stop(coder->table))
		return (0);
	pthread_mutex_lock(&coder->table->print_lock);
	printf("\033[33m%ld %d is debugging\033[0m\n", gettimenow()
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	smart_sleep(coder->table->time_to_debug, coder->table);
	return (1);
}

int	refactoring(t_coder *coder)
{
	if (get_stop(coder->table))
		return 0;
	pthread_mutex_lock(&coder->table->print_lock);
	printf("\033[33m%ld %d is refactoring\033[0m\n", gettimenow()
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	smart_sleep(coder->table->time_to_refactor, coder->table);
	return 1;
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_stop(coder->table))
	{
		if (get_stop(coder->table) || is_complet_compile(coder) || !compile(coder))
			break ;
		if (get_stop(coder->table) || is_complet_compile(coder) || !debugging(coder))
			break ;
		if (get_stop(coder->table) || is_complet_compile(coder) || !refactoring(coder))
			break ;
	}
	return (NULL);
}
