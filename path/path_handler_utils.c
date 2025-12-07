/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:39:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/03 21:52:36 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path_cmd(const char *path, const char *cmd, t_data *data)
{
	char	*full_path;
	size_t	len_path;
	size_t	len_cmd;

	len_path = ft_strlen(path);
	len_cmd = ft_strlen(cmd);
	full_path = gc_malloc(len_path + 1 + len_cmd + 1, data);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, path, len_path);
	full_path[len_path] = '/';
	ft_memcpy(full_path + len_path + 1, cmd, len_cmd);
	full_path[len_path + 1 + len_cmd] = '\0';
	return (full_path);
}

char	*try_current_dir(char *cmd_name, t_data *data)
{
	char	current_dir[PATH_MAX];
	char	*final_path;

	if (getcwd(current_dir, PATH_MAX) == NULL)
		return (NULL);
	final_path = join_path_cmd(current_dir, cmd_name, data);
	if (final_path && access(final_path, F_OK | X_OK) == 0)
		return (final_path);
	gc_free_ptr(final_path, data);
	return (NULL);
}

char	*check_paths_for_cmd(char **paths, char *cmd_name, t_data *data)
{
	int		i;
	char	*full_path_attempt;

	i = 0;
	while (paths && paths[i])
	{
		full_path_attempt = join_path_cmd(paths[i], cmd_name, data);
		if (full_path_attempt)
		{
			if (access(full_path_attempt, X_OK) == 0)
				return (full_path_attempt);
			gc_free_ptr(full_path_attempt, data);
		}
		i++;
	}
	return (NULL);
}
