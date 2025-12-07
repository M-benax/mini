/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/06 23:17:03 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_pid_list	*add_pid_to_list(t_pid_list *list_head, pid_t pid, t_data *data)
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

void	wait_for_pipeline_pids(t_pid_list *pid_list_head, pid_t last_cmd_pid,
			t_data *data)
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

pid_t	fork_pipeline_segment(t_cmd *cmd, t_data *data, int fd_in, int fda[2])
{
	pid_t	pid;

	pid = fork();
	g_sigint_received = 700;
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (fda[0] != -1)
			close(fda[0]);
		signal_init();
		if (fd_in != STDIN_FILENO)
			apply_redirection(fd_in, STDIN_FILENO);
		if (fda[1] != STDOUT_FILENO)
			apply_redirection(fda[1], STDOUT_FILENO);
		execute_command_in_child(cmd, data);
	}
	return (pid);
}

void	handle_pipe_error(int input_fd, t_pid_list *pid_list_head, t_data *data)
{
	perror("minishell: pipe");
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	gc_free_pid_list(pid_list_head, data);
	data->last_exit_status = EXIT_GENERAL_ERROR;
}

void	cleanup_heredoc(t_cmd *cmd)
{
	if (cmd->in_type == T_HEREDOC && cmd->in_file)
	{
		if (access(cmd->in_file, F_OK) == 0)
			unlink(cmd->in_file);
	}
}
