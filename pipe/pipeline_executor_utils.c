/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:20:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/06 23:07:55 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe_cmd(t_cmd *cmd, t_data *data, int *input_fd)
{
	int		pipe_fds[2];
	int		fda[2];
	pid_t	last_pid;

	if (pipe(pipe_fds) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	fda[0] = pipe_fds[0];
	fda[1] = pipe_fds[1];
	last_pid = fork_pipeline_segment(cmd, data, *input_fd, fda);
	close(pipe_fds[1]);
	if (last_pid == -1)
	{
		close(pipe_fds[0]);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
		return (-1);
	}
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	*input_fd = pipe_fds[0];
	return (last_pid);
}

int	handle_last_cmd(t_cmd *cmd, t_data *data, int input_fd)
{
	pid_t	last_pid;
	int		fda[2];

	fda[0] = -1;
	fda[1] = STDOUT_FILENO;
	last_pid = fork_pipeline_segment(cmd, data, input_fd, fda);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (last_pid);
}

int	process_cmd(t_cmd *cmd, t_data *data, int *input_fd)
{
	if (cmd->next)
		return (handle_pipe_cmd(cmd, data, input_fd));
	else
		return (handle_last_cmd(cmd, data, *input_fd));
}

void	pipeline_cleanup(t_pid_list *pid_list_head, pid_t last_pid,
	t_data *data)
{
	t_cmd	*cmd;

	wait_for_pipeline_pids(pid_list_head, last_pid, data);
	cmd = data->com;
	while (cmd)
	{
		cleanup_heredoc(cmd);
		cmd = cmd->next;
	}
	gc_free_pid_list(pid_list_head, data);
}

t_pid_list	*process_pipeline_cmds(t_data *data, int *input_fd,
	pid_t *last_pid)
{
	t_cmd		*current_cmd;
	t_pid_list	*pid_list_head;

	current_cmd = data->com;
	pid_list_head = NULL;
	while (current_cmd)
	{
		if (current_cmd->has_redirect_error)
		{
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return (NULL);
		}
		*last_pid = process_cmd(current_cmd, data, input_fd);
		if (*last_pid == -1)
		{
			gc_free_pid_list(pid_list_head, data);
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return (NULL);
		}
		pid_list_head = add_pid_to_list(pid_list_head, *last_pid, data);
		current_cmd = current_cmd->next;
	}
	return (pid_list_head);
}
