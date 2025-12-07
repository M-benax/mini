/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/06 23:24:10 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_no_executable(t_cmd *cmd, t_data *data)
{
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) == 0)
		{
			display_error_message(cmd->args[0], NULL, "Permission denied");
			exit(EXIT_CMD_CANNOT_EXECUTE);
		}
		else
		{
			display_error_message(cmd->args[0], NULL,
				"No such file or directory");
			exit(EXIT_CMD_NOT_FOUND);
		}
	}
	else
		handle_path_error(cmd, data);
}

void	execute_with_path(t_cmd *cmd, t_data *data, char *executable_path)
{
	char	**envp_array;

	envp_array = convert_env_list_to_array(data->tenv, data);
	if (!envp_array)
	{
		gc_free_ptr(executable_path, data);
		exit(EXIT_GENERAL_ERROR);
	}
	execve(executable_path, cmd->args, envp_array);
	display_error_message(cmd->args[0], NULL, strerror(errno));
	gc_free_ptr(executable_path, data);
	gc_free_array(envp_array, data);
	if (errno == EACCES)
		exit(EXIT_CMD_CANNOT_EXECUTE);
	if (errno == ENOENT)
		exit(EXIT_CMD_NOT_FOUND);
	exit(EXIT_GENERAL_ERROR);
}
