/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:50:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/08 17:33:38 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_heredoc_eof_msg(char *limiter)
{
	write(STDERR_FILENO, "minishell: warning: here-document delimited by "
		"end-of-file ", 54);
	write(STDERR_FILENO, "(wanted `", 9);
	write(STDERR_FILENO, limiter, ft_strlen(limiter));
	write(STDERR_FILENO, "')\n", 3);
}

void	process_heredoc_line(int fd, t_cmd *cmd, char *line, t_data *data)
{
	char	*expanded_line;

	expanded_line = expand_line(line, cmd, data);
	if (expanded_line)
	{
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		gc_free_ptr(expanded_line, data);
	}
	free(line);
}

void	handle_heredoc_child(int fd, t_cmd *cmd, char *limiter, t_data *data)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			break ;
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(fd, cmd, line, data);
	}
	close(fd);
	exit(0);
}
