/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:05:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/03 22:34:59 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_signals_and_close(int fd, void (*old_sigint)(int),
		void (*old_sigquit)(int))
{
	close(fd);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
}

int	setup_heredoc_fork(int *fd, char **file_name, t_data *data,
		void (**old_handlers)(int))
{
	*fd = open_file(file_name, data);
	if (*fd < 0)
		return (-1);
	old_handlers[0] = signal(SIGINT, SIG_IGN);
	old_handlers[1] = signal(SIGQUIT, SIG_IGN);
	return (fork());
}

int	handle_heredoc_parent(pid_t pid, char *file_name, t_cmd *cmd,
		void (**old_handlers)(int))
{
	int	status;

	if (pid < 0)
	{
		unlink(file_name);
		return (1);
	}
	waitpid(pid, &status, 0);
	signal(SIGINT, old_handlers[0]);
	signal(SIGQUIT, old_handlers[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(file_name);
		return (1);
	}
	if (access(file_name, R_OK) != 0)
	{
		unlink(file_name);
		return (1);
	}
	cmd->in_file = file_name;
	return (0);
}
