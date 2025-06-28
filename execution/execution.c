/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:09 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:10:13 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_slash_path(t_cmd *cmd)
{
	if (access(cmd->args[0], F_OK) == 0)
	{
		display_error_message(cmd->args[0], NULL, "Permission denied");
		exit(EXIT_CMD_CANNOT_EXECUTE);
	}
	else
	{
		display_error_message(cmd->args[0], NULL, "No such file or directory");
		exit(EXIT_CMD_NOT_FOUND);
	}
}

static void	handle_no_path(t_cmd *cmd, t_data *data)
{
	char	*path_env;

	path_env = get_env_value(data->tenv, "PATH", data);
	if (!path_env || path_env[0] == '\0')
	{
		if (access(cmd->args[0], F_OK) == 0)
			display_error_message(cmd->args[0], NULL, "Permission denied");
		else
			display_error_message(cmd->args[0],
				NULL, "No such file or directory");
		if (path_env)
			gc_free_ptr(path_env, data);
	}
	else
	{
		display_error_message(cmd->args[0], NULL, "command not found");
		gc_free_ptr(path_env, data);
	}
	exit(EXIT_CMD_NOT_FOUND);
}

static void	handle_path_error(t_cmd *cmd, t_data *data)
{
	if (ft_strchr(cmd->args[0], '/'))
		handle_slash_path(cmd);
	else
		handle_no_path(cmd, data);
}

static void	execute_external_cmd(char *executable_path,
			t_cmd *cmd, t_data *data)
{
	char	**envp_array;

	envp_array = convert_env_list_to_array(data->tenv, data);
	if (!envp_array)
	{
		gc_free_ptr(executable_path, data);
		exit(EXIT_GENERAL_ERROR);
	}
	execve(executable_path, cmd->args, envp_array);
	display_error_message(cmd->args[0], NULL, strerror(errno));
	gc_free_ptr(executable_path, data);
	gc_free_array(envp_array, data);
	if (errno == EACCES)
		exit(EXIT_CMD_CANNOT_EXECUTE);
	if (errno == ENOENT)
		exit(EXIT_CMD_NOT_FOUND);
	exit(EXIT_GENERAL_ERROR);
}

void	execute_command_in_child(t_cmd *cmd, t_data *data)
{
	char	*executable_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_child_redirections(cmd) != 0)
		exit(EXIT_GENERAL_ERROR);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		exit(execute_builtin_child(cmd, data));
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	executable_path = find_executable_path(cmd->args[0], data->tenv, data);
	if (!executable_path)
		handle_path_error(cmd, data);
	execute_external_cmd(executable_path, cmd, data);
}
