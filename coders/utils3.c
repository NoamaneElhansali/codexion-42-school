/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:49:47 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/18 21:28:36 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	free_allocate_init(t_table *t)
{
	if (!t)
		return (0);
	free(t->dongles);
	free(t->coders);
	free(t);
	return (1);
}

t_table	*check_init(t_table *table, int *parameter)
{
	if (table->nb_coders * sizeof(t_dongle) > (INT_MAX)-1 || table->nb_coders
		* sizeof(t_coder) > (INT_MAX)-1)
		return (print_message_err("[ERROR MEMORY]", NULL), free(parameter),
			free(table), NULL);
	table->dongles = malloc(sizeof(t_dongle) * table->nb_coders);
	table->coders = malloc(sizeof(t_coder) * table->nb_coders);
	if (!table->dongles || !table->coders)
		return (print_message_err("[ERROR MEMORY]", NULL), free(parameter),
			free_allocate_init(table), NULL);
	if (!init_mutex(table) || !init_mutex_coder(table) || !init_dongles(table)
		|| !init_coders(table))
		return (print_message_err("[ERROR MEMORY]", NULL), free(parameter),
			free_allocate_init(table), NULL);
	return (table);
}
