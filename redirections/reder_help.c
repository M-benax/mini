/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reder_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:36:56 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/07 22:29:20 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_out_redirection(char *out_file, int out_type)
{
	int	fd;

	if (!out_file)
		return (0);
	fd = open_output_file(out_file, out_type);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_in_redirection(char *in_file)
{
	int	fd;

	if (!in_file)
		return (0);
	fd = open_input_file(in_file);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_parent_redirections(t_cmd *cmd)
{
	if (handle_in_redirection(cmd->in_file))
		return (1);
	if (handle_out_redirection(cmd->out_file, cmd->out_type))
		return (1);
	return (0);
}
