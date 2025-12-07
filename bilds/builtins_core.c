/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:46:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/12 21:23:08 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin_parent(t_cmd *cmd, t_data *data)
{
	int	exit_status;
	int	stdout_fd;
	int	stdin_fd;

	exit_status = 0;
	stdout_fd = -1;
	stdin_fd = -1;
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		data->last_exit_status = EXIT_CMD_NOT_FOUND;
		return ;
	}
	if (cmd->out_file || cmd->in_file)
	{
		handle_builtin_redirections(cmd, &stdout_fd, &stdin_fd);
		if (handle_parent_redirections(cmd) != 0)
		{
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return ;
		}
	}
	exit_status = exec_builtin_cmd(cmd, data);
	restore_builtin_redirections(stdout_fd, stdin_fd);
	if (ft_strcmp(cmd->args[0], "exit") != 0)
		data->last_exit_status = exit_status;
}

int	execute_builtin_child(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (EXIT_CMD_NOT_FOUND);
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
