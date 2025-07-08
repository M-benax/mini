/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:39:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/29 18:44:39 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_pipe_cmd(t_cmd *cmd, t_data *data, int *input_fd)
{
	int		pipe_fds[2];
	pid_t	last_pid;

	if (pipe(pipe_fds) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	last_pid = fork_pipeline_segment(cmd, data, *input_fd, pipe_fds[1]);
	close(pipe_fds[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	*input_fd = pipe_fds[0];
	return (last_pid);
}

static int	handle_last_cmd(t_cmd *cmd, t_data *data, int input_fd)
{
	pid_t	last_pid;

	last_pid = fork_pipeline_segment(cmd, data, input_fd, STDOUT_FILENO);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (last_pid);
}

static int	process_cmd(t_cmd *cmd, t_data *data, int *input_fd)
{
	if (cmd->next)
		return (handle_pipe_cmd(cmd, data, input_fd));
	else
		return (handle_last_cmd(cmd, data, *input_fd));
}

void	execute_pipeline(t_data *data)
{
	t_cmd		*current_cmd;
	int			input_fd;
	t_pid_list	*pid_list_head;
	pid_t		last_pid_in_pipeline;

	input_fd = STDIN_FILENO;
	current_cmd = data->com;
	pid_list_head = NULL;
	while (current_cmd)
	{
		last_pid_in_pipeline = process_cmd(current_cmd, data, &input_fd);
		if (last_pid_in_pipeline == -1)
		{
			gc_free_pid_list(pid_list_head, data);
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return ;
		}
		pid_list_head = add_pid_to_list(pid_list_head, last_pid_in_pipeline,
				data);
		cleanup_heredoc(current_cmd);
		current_cmd = current_cmd->next;
	}
	wait_for_pipeline_pids(pid_list_head, last_pid_in_pipeline, data);
	gc_free_pid_list(pid_list_head, data);
}
