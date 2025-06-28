/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:40 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 19:36:41 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static int is_valid_n_flag(char *arg)
{
	int i;
	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int builtin_echo(char **args)
{
	int i = 1;
	int n_option = 0;

	while (args[i] && is_valid_n_flag(args[i]))
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i+1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}