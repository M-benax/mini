/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:11:15 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:11:17 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_str_numeric(const char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_numeric_arg(char **args, int *exit_code_val, t_data *data)
{
	if (!is_str_numeric(args[1]))
	{
		display_error_message("exit", args[1], "numeric argument required");
		*exit_code_val = EXIT_SYNTAX_ERROR;
		gc_free_all(data);
		exit(*exit_code_val);
	}
	*exit_code_val = ft_atoi(args[1]) % 256;
	if (*exit_code_val < 0)
		*exit_code_val += 256;
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_code_val;
	int	arg_count;

	arg_count = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	exit_code_val = data->last_exit_status;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 1)
	{
		handle_numeric_arg(args, &exit_code_val, data);
		if (arg_count > 2)
		{
			display_error_message("exit", NULL, "too many arguments");
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return (EXIT_GENERAL_ERROR);
		}
	}
	gc_free_all(data);
	exit(exit_code_val);
	return (0);
}
