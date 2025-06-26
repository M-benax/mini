/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:02:24 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 15:16:28 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handel_heredoc(char *limiter, t_cmd *cmd, t_data *data)
{
	int fd;
	char *line;
	char *file_name;
	char *expanded_line;

	fd = open_file(&file_name, data);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		expanded_line = expand_line(line, data);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
		free(line);
	}
	close(fd);
	cmd->in_file = file_name;
}
void	copy_args(char **src, char **dst, int count)
{
	int i;

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