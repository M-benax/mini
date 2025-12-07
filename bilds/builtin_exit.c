/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/10 14:17:05 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_str_numeric(const char *str)
{
	int	i;
	int	has_digits;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	has_digits = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		has_digits = 1;
		i++;
	}
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (has_digits && str[i] == '\0');
}

static int	handle_exit_args(char **args, t_data *data)
{
	int	arg_count;
	int	exit_code_val;

	arg_count = count_args(args);
	exit_code_val = data->last_exit_status;
	if (arg_count > 1)
	{
		if (!is_str_numeric(args[1]))
		{
			display_error_message("exit", args[1], "numeric argument required");
			exit_code_val = EXIT_SYNTAX_ERROR;
			gc_free_all(data);
			exit(exit_code_val);
		}
		if (arg_count > 2)
		{
			display_error_message("exit", NULL, "too many arguments");
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return (EXIT_GENERAL_ERROR);
		}
		exit_code_val = ft_atoi(args[1]) % 256;
		if (exit_code_val < 0)
			exit_code_val += 256;
	}
	return (exit_code_val);
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_code_val;

	write(STDOUT_FILENO, "exit\n", 5);
	exit_code_val = handle_exit_args(args, data);
	if (exit_code_val == EXIT_GENERAL_ERROR)
		return (EXIT_GENERAL_ERROR);
	gc_free_all(data);
	exit(exit_code_val);
	return (0);
}
