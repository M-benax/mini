/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:45 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:10:48 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	if (ft_strcmp(cmd_name, "echo") == 0
		|| ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0
		|| ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	return (0);
}

static int	handle_env_export_unset(t_cmd *cmd, t_data *data)
{
	int	status_code;

	status_code = 0;
	if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		if (!cmd->args[1])
			print_env(data->tenv);
		else
		{
			display_error_message("env", cmd->args[1], "No such file");
			status_code = EXIT_CMD_CANNOT_EXECUTE;
		}
	}
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (!cmd->args[1])
			print_export(data->tenv, data);
		else
			status_code = handel_export(data);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		status_code = unset(cmd->args, &(data->tenv));
	return (status_code);
}

static int	execute_parent_builtin(t_cmd *cmd, t_data *data)
{
	int	status_code;

	status_code = 0;
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, data);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		status_code = builtin_cd(cmd->args, &(data->tenv), data);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		status_code = builtin_pwd(data->tenv, data);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		status_code = builtin_echo(cmd->args);
	else
		status_code = handle_env_export_unset(cmd, data);
	return (status_code);
}

static int	setup_and_restore_redirections(t_cmd *cmd, t_data *data)
{
	int	stdout_backup;
	int	stdin_backup;
	int	status_code;

	stdout_backup = -1;
	stdin_backup = -1;
	if (cmd->out_file)
		stdout_backup = dup(STDOUT_FILENO);
	if (cmd->in_file)
		stdin_backup = dup(STDIN_FILENO);
	if (handle_child_redirections(cmd) != 0)
		return (EXIT_GENERAL_ERROR);
	status_code = execute_parent_builtin(cmd, data);
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
	return (status_code);
}

void	execute_builtin_parent(t_cmd *cmd, t_data *data)
{
	int	status_code;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		data->last_exit_status = EXIT_CMD_NOT_FOUND;
		return ;
	}
	if (cmd->out_file || cmd->in_file)
		status_code = setup_and_restore_redirections(cmd, data);
	else
		status_code = execute_parent_builtin(cmd, data);
	if (status_code == EXIT_GENERAL_ERROR)
		data->last_exit_status = EXIT_GENERAL_ERROR;
	else if (ft_strcmp(cmd->args[0], "exit") != 0)
		data->last_exit_status = status_code;
}
