/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nelhansa <nelhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:04 by nelhansa          #+#    #+#             */
/*   Updated: 2026/04/18 21:09:53 by nelhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parser_parameter(char **av, int ac)
{
	int		i;
	long	numbre;

	i = 1;
	if (ac != 9)
		return (0);
	while (i < ac - 1)
	{
		numbre = ft_atoi(av[i++]);
		if (numbre < 0 || numbre > INT_MAX)
			return (0);
	}
	if (!ft_strcmp_adv(av[8], "fifo") && !ft_strcmp_adv(av[8], "edf"))
		return (0);
	return (1);
}

int	*parser(char **av, int ac)
{
	int		i;
	int		*param;
	long	val;

	i = 1;
	param = malloc(sizeof(int) * 7);
	if (!param)
		return (NULL);
	while (i < ac - 1)
	{
		val = ft_atoi(av[i]);
		if (val < 0 || val > INT_MAX)
		{
			free(param);
			return (NULL);
		}
		param[i - 1] = (int)val;
		i++;
	}
	return (param);
}

void	print_message_err(char *msg, t_table *t)
{
	if (!t)
	{
		printf("-------\n%s\n--------\n", msg);
		return ;
	}
	set_stop(t);
	pthread_mutex_lock(&t->print_lock);
	printf("-------\n%s\n--------\n", msg);
	pthread_mutex_unlock(&t->print_lock);
}
