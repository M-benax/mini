/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:39:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/10 15:00:26 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_absolute_path(char *cmd_name, t_data *data)
{
	if (access(cmd_name, F_OK) == 0)
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name, data));
		return (NULL);
	}
	return (NULL);
}

static char	*try_path_env(char *cmd_name, t_env *env_list, t_data *data)
{
	char	*path_env_value;
	char	**paths_array;
	char	*final_path;

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

char	*find_executable_path(char *cmd_name, t_env *env_list, t_data *data)
{
	char	*final_path;

	if (!cmd_name || cmd_name[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
		return (handle_absolute_path(cmd_name, data));
	final_path = try_path_env(cmd_name, env_list, data);
	if (final_path)
		return (final_path);
	return (try_current_dir(cmd_name, data));
}
