/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:39:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/25 18:57:33 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

static char *join_path_cmd(const char *path, const char *cmd, t_data *data)
{
	char *full_path;
	size_t len_path = ft_strlen(path);
	size_t len_cmd = ft_strlen(cmd);

	full_path = gc_malloc(len_path + 1 + len_cmd + 1, data);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, path, len_path);
	full_path[len_path] = '/';
	ft_memcpy(full_path + len_path + 1, cmd, len_cmd);
	full_path[len_path + 1 + len_cmd] = '\0';
	return (full_path);
}

static char *check_paths_for_cmd(char **paths, char *cmd_name, t_data *data)
{
	int i = 0;
	char *full_path_attempt;

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

char *find_executable_path(char *cmd_name, t_env *env_list, t_data *data)
{
	char *path_env_value;
	char **paths_array;
	char *final_path;

	if (!cmd_name || cmd_name[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, F_OK) == 0)
		{
			if (access(cmd_name, X_OK) == 0)
				return (ft_strdup(cmd_name, data));
			return (NULL); /* File exists but not executable */
		}
		return (NULL); /* File doesn't exist */
	}
	path_env_value = get_env_value(env_list, "PATH", data);
	if (!path_env_value || path_env_value[0] == '\0')
	{
		gc_free_ptr(path_env_value, data);
		return (NULL);
	}
	paths_array = ft_split(path_env_value, ':', data);
	gc_free_ptr(path_env_value, data);
	if (!paths_array)
		return (NULL);
	final_path = check_paths_for_cmd(paths_array, cmd_name, data);
	gc_free_array(paths_array, data);
	return (final_path);
}