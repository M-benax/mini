/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_03.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:29 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/08 16:55:23 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_heredoc_file(t_cmd *cmd)
{
	if (cmd->in_type == T_HEREDOC && cmd->in_file)
	{
		if (access(cmd->in_file, F_OK) == 0)
			unlink(cmd->in_file);
	}
}

static void	handle_parent_process(pid_t pid, t_cmd *cmd, t_data *data)
{
	handle_parent_wait(pid, 1, data);
	cleanup_heredoc_file(cmd);
}

static pid_t	try_fork_and_handle(t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		data->last_exit_status = EXIT_GENERAL_ERROR;
	}
	return (pid);
}

void	execute_single_external_command(t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	pid = try_fork_and_handle(data);
	g_sigint_received = 600;
	if (pid == -1)
		return ;
	if (pid == 0)
		execute_command_in_child(cmd, data);
	else
		handle_parent_process(pid, cmd, data);
}
