/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:25:58 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/12 03:13:50 by nelhansa         ###   ########.fr       */
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
	pthread_mutex_destroy(&table->queue_lock);
	pthread_cond_destroy(&table->cond);
	while (i < table->nb_coders)
	{
		pthread_mutex_destroy(&table->dongles[i].mutex);
		pthread_mutex_destroy(&table->coders[i].compile_count_lock);
		pthread_mutex_destroy(&table->coders[i].last_compile_lock);
		i++;
	}
}

int	check_parameter(int *p)
{
	if (p[0] == 0 || p[1] == 0 || p[5] == 0)
	{
		free(p);
		return (0);
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
	start_monitoring(table);
	destroy_mutex_cond(table);
	free(param);
	free(table->coders);
	free(table->queue.data);
	free(table->heap.data);
	free(table->dongles);
	free(table);
	return (0);
}
