/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:40:10 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/08 15:33:54 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_output_file(char *filename, int type)
{
	int	flags;
	int	fd;

	if (type == T_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd == -1)
		display_error_message(NULL, filename, strerror(errno));
	return (fd);
}

int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		display_error_message(NULL, filename, strerror(errno));
	return (fd);
}

void	apply_redirection(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		close(old_fd);
		perror("minishell: dup2");
		exit(EXIT_FAILURE);
	}
	close(old_fd);
}

int	handle_child_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->in_file)
	{
		fd = open_input_file(cmd->in_file);
		if (fd == -1)
			return (1);
		apply_redirection(fd, STDIN_FILENO);
	}
	if (cmd->out_file)
	{
		fd = open_output_file(cmd->out_file, cmd->out_type);
		if (fd == -1)
			return (1);
		apply_redirection(fd, STDOUT_FILENO);
	}
	return (0);
}
