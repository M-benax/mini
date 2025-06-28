/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core_02.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:59 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:11:01 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_child_builtins(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(data->tenv, data));
	if (ft_strcmp(cmd->args[0], "env") == 0 && !cmd->args[1])
	{
		print_env(data->tenv);
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "export") == 0 && !cmd->args[1])
	{
		print_export(data->tenv, data);
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, data);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd->args, &(data->tenv), data));
	if (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1])
		return (handel_export(data));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset(cmd->args, &(data->tenv)));
	return (EXIT_CMD_NOT_FOUND);
}

int	execute_builtin_child(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (EXIT_CMD_NOT_FOUND);
	return (handle_child_builtins(cmd, data));
}
