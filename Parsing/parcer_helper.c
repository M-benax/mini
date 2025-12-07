/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:02:24 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/03 21:04:08 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handel_heredoc(char *limiter, t_cmd *cmd, t_data *data)
{
	int		fd;
	char	*file_name;
	pid_t	pid;
	void	(*old_handlers[2])(int);
	int		result;

	pid = setup_heredoc_fork(&fd, &file_name, data, old_handlers);
	if (pid == -1)
	{
		perror("fork");
		restore_signals_and_close(fd, old_handlers[0], old_handlers[1]);
		return (1);
	}
	if (pid == 0)
		handle_heredoc_child(fd, cmd, limiter, data);
	close(fd);
	result = handle_heredoc_parent(pid, file_name, cmd, old_handlers);
	if (result == 1)
		data->last_exit_status = 1;
	return (result);
}

void	copy_args(char **src, char **dst, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}

void	copy_flags(int *src, int *dst, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}
