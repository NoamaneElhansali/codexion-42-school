/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:07 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 02:19:35 by nelhansa         ###   ########.fr       */
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

void	compile(t_coder *coder)
{
	long	now;

	if (coder->table->scheduler == FIFO)
	{
		if (!take_dongles_fifo(coder))
			return ;
	}
	else
	{
		if (!take_dongles_edf(coder))
			return ;
	}
	if (get_stop(coder->table))
	{
		give_dongles(coder);
		return ;
	}
	now = gettimenow();
	compile_suit(coder, now);
}

void	debugging(t_coder *coder)
{
	if (get_stop(coder->table))
		return ;
	pthread_mutex_lock(&coder->table->print_lock);
	printf("\033[33m%ld %d is debugging\033[0m\n", gettimenow()
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	smart_sleep(coder->table->time_to_debug, coder->table);
}

void	refactoring(t_coder *coder)
{
	if (get_stop(coder->table))
		return ;
	pthread_mutex_lock(&coder->table->print_lock);
	printf("\033[33m%ld %d is refactoring\033[0m\n", gettimenow()
		- coder->table->start_time, coder->id);
	pthread_mutex_unlock(&coder->table->print_lock);
	smart_sleep(coder->table->time_to_refactor, coder->table);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_stop(coder->table))
	{
		// smart_sleep(coder->table->dongle_cooldown, coder->table);
		if (get_stop(coder->table) || is_complet_compile(coder))
			break ;
		compile(coder);
		if (get_stop(coder->table) || is_complet_compile(coder))
			break ;
		debugging(coder);
		if (get_stop(coder->table) || is_complet_compile(coder))
			break ;
		refactoring(coder);
	}
	return (NULL);
}
