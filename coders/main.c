/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:58 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/14 12:37:48 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_complet_compile(t_coder *coder)
{
	return (get_compile_count(coder) == coder->table->must_compile);
}

void	destroy_mutex_cond(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->stop_lock);
	while (i < table->nb_coders)
	{
		pthread_mutex_destroy(&table->dongles[i].mutex);
		pthread_mutex_destroy(&table->dongles[i].queue_lock);
		pthread_cond_destroy(&table->dongles[i].cond);
		pthread_mutex_destroy(&table->coders[i].compile_count_lock);
		pthread_mutex_destroy(&table->coders[i].last_compile_lock);
		i++;
	}
}

int	check_parameter(int *p)
{
	if (!p)
		return (0);
	if (p[0] == 0 || p[1] == 0 || p[5] == 0)
	{
		free(p);
		return (0);
	}
	return (1);
}

int	free_allocate(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->nb_coders)
	{
		if (t->scheduler == FIFO)
			free(t->dongles[i].queue.data);
		else
			free(t->dongles[i].heap.data);
		i++;
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	int		*param;
	t_table	*table;

	if (!parser_parameter(argv, argc))
		return (write(2, "[INVALID ARG]\n", 14));
	param = parser(argv, argc);
	if (!check_parameter(param))
		return (write(2, "[INVALID ARG]\n", 14));
	table = init_parameter(param, argv[8]);
	if (!table)
		return (1);
	start_monitoring(table);
	destroy_mutex_cond(table);
	free(param);
	free(table->coders);
	free_allocate(table);
	free(table->dongles);
	free(table);
	return (0);
}
