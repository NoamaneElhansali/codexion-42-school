/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:49:47 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/14 13:52:12 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_table	*check_init(t_table *table)
{
	if (!table->dongles || !table->coders)
		return (print_message_err("[ERROR MEMORY]", table), NULL);
	if (!init_mutex(table) || !init_mutex_coder(table) || !init_dongles(table)
		|| !init_coders(table))
		return (print_message_err("[ERROR]", table), NULL);
	return (table);
}
