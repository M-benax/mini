/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:09:48 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:09:52 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_pid_list	*add_pid_to_list(t_pid_list *list_head, pid_t pid,
	t_data *data)
{
	t_pid_list	*new_node;
	t_pid_list	*current;

	new_node = gc_malloc(sizeof(t_pid_list), data);
	if (!new_node)
		return (list_head);
	new_node->pid = pid;
	new_node->next = NULL;
	if (!list_head)
		return (new_node);
	current = list_head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (list_head);
}

static void	wait_for_pipeline_pids(t_pid_list *pid_list_head,
	pid_t last_cmd_pid, t_data *data)
{
	t_pid_list	*current_node;
	int			is_last_in_pipeline;

	current_node = pid_list_head;
	while (current_node)
	{
		is_last_in_pipeline = (current_node->pid == last_cmd_pid);
		handle_parent_wait(current_node->pid, is_last_in_pipeline, data);
		current_node = current_node->next;
	}
}

static pid_t	fork_and_execute(t_cmd *cmd, t_data *data,
		int fd_in, int fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (fd_in != STDIN_FILENO)
			apply_redirection(fd_in, STDIN_FILENO);
		if (fd_out != STDOUT_FILENO)
			apply_redirection(fd_out, STDOUT_FILENO);
		execute_command_in_child(cmd, data);
	}
	return (pid);
}

static pid_t	process_pipeline_command(t_cmd *cmd,
		t_data *data, int *input_fd,
	t_pid_list **pid_list_head)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (cmd->next)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe");
			data->last_exit_status = EXIT_GENERAL_ERROR;
			return (-1);
		}
		pid = fork_and_execute(cmd, data, *input_fd, pipe_fds[1]);
		close(pipe_fds[1]);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
		*input_fd = pipe_fds[0];
	}
	else
	{
		pid = fork_and_execute(cmd, data, *input_fd, STDOUT_FILENO);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
	}
	*pid_list_head = add_pid_to_list(*pid_list_head, pid, data);
	return (pid);
}

void	execute_pipeline(t_data *data)
{
	t_cmd		*current_cmd;
	int			input_fd;
	t_pid_list	*pid_list_head;
	pid_t		last_pid;

	input_fd = STDIN_FILENO;
	current_cmd = data->com;
	pid_list_head = NULL;
	last_pid = 0;
	while (current_cmd)
	{
		last_pid = process_pipeline_command(current_cmd, data, &input_fd,
				&pid_list_head);
		if (last_pid == -1)
			return ;
		if (current_cmd->in_type == T_HEREDOC && current_cmd->in_file)
		{
			if (access(current_cmd->in_file, F_OK) == 0)
				unlink(current_cmd->in_file);
		}
		current_cmd = current_cmd->next;
	}
	wait_for_pipeline_pids(pid_list_head, last_pid, data);
	gc_free_pid_list(pid_list_head, data);
}
