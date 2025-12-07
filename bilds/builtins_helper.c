/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/14 02:43:33 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exec_basic_builtins(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, data);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd->args, &(data->tenv), data));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(data->tenv, data));
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	return (-1);
}

static int	exec_env_builtins(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->args[0], "env") == 0 && !cmd->args[1])
		print_env(data->tenv);
	else if (ft_strcmp(cmd->args[0], "export") == 0 && !cmd->args[1])
		print_export(data->tenv, data);
	else if (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1])
		return (handel_export(data));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset(cmd->args, &(data->tenv)));
	else
		return (-1);
	return (0);
}

int	exec_builtin_cmd(t_cmd *cmd, t_data *data)
{
	int	result;

	result = exec_basic_builtins(cmd, data);
	if (result != -1)
		return (result);
	return (exec_env_builtins(cmd, data));
}

void	handle_builtin_redirections(t_cmd *cmd, int *stdout_backup,
			int *stdin_backup)
{
	if (cmd->out_file)
		*stdout_backup = dup(STDOUT_FILENO);
	if (cmd->in_file)
		*stdin_backup = dup(STDIN_FILENO);
}

void	restore_builtin_redirections(int stdout_backup, int stdin_backup)
{
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
}
