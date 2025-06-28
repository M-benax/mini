/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:20 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:10:22 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_parent_wait(pid_t child_pid,
		int is_pipeline_last_cmd, t_data *data)
{
	int	status;
	int	child_exit_status;

	waitpid(child_pid, &status, 0);
	child_exit_status = 0;
	if (WIFEXITED(status))
		child_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		child_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit: 3\n", 8);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	if (is_pipeline_last_cmd)
		data->last_exit_status = child_exit_status;
}
