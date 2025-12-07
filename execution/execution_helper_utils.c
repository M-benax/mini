/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:40:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/08 16:55:06 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	try_current_directory(t_cmd *cmd, t_data *data)
{
	char	current_dir[PATH_MAX];
	char	*full_path;

	if (getcwd(current_dir, PATH_MAX) != NULL)
	{
		full_path = join_path_cmd(current_dir, cmd->args[0], data);
		if (full_path)
		{
			if (access(full_path, F_OK) == 0)
			{
				if (access(full_path, X_OK) != 0)
					display_error_message(cmd->args[0],
						NULL, "Permission denied");
				else
					execute_with_path(cmd, data, full_path);
				gc_free_ptr(full_path, data);
				exit(EXIT_CMD_CANNOT_EXECUTE);
			}
			gc_free_ptr(full_path, data);
		}
	}
	return (0);
}

void	handle_path_error(t_cmd *cmd, t_data *data)
{
	char	*path_env;

	path_env = get_env_value(data->tenv, "PATH", data);
	if (!path_env || path_env[0] == '\0')
	{
		try_current_directory(cmd, data);
		display_error_message(cmd->args[0], NULL, "command not found");
		if (path_env)
			gc_free_ptr(path_env, data);
	}
	else
	{
		display_error_message(cmd->args[0], NULL, "command not found");
		gc_free_ptr(path_env, data);
	}
	exit(EXIT_CMD_NOT_FOUND);
}
