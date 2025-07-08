/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:38:11 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 19:38:12 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_parent_wait(pid_t child_pid, int is_pipeline_last_cmd,
			t_data *data)
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

void	display_error_message(char *command, char *arg, char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (command)
	{
		write(STDERR_FILENO, command, ft_strlen(command));
		write(STDERR_FILENO, ": ", 2);
	}
	if (arg)
	{
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}
