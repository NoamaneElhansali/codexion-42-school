/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:13 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 02:18:31 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	gettimenow(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_last_compile(t_coder *coder)
{
	long	value;

	pthread_mutex_lock(&coder->last_compile_lock);
	value = coder->last_compile;
	pthread_mutex_unlock(&coder->last_compile_lock);
	return (value);
}

void	set_last_compile(t_coder *coder, long time)
{
	pthread_mutex_lock(&coder->last_compile_lock);
	coder->last_compile = time;
	pthread_mutex_unlock(&coder->last_compile_lock);
}

int	get_compile_count(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->compile_count_lock);
	i = coder->compile_count;
	pthread_mutex_unlock(&coder->compile_count_lock);
	return (i);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->compile_count_lock);
	coder->compile_count++;
	if (coder->compile_count == coder->table->must_compile)
	{
		pthread_mutex_lock(&coder->table->complite_sim_lock);
		// printf("// => %d\n", coder->compile_count);
		coder->table->complite_sim++;
		pthread_mutex_unlock(&coder->table->complite_sim_lock);
	}
	pthread_mutex_unlock(&coder->compile_count_lock);
}
