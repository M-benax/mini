/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_03.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:29 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/28 20:14:38 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	start_execution(t_data *data)
{
	if (!data || !data->com)
		return ;
	if (!data->com->args || !data->com->args[0] || data->com->argc == 0)
	{
		data->last_exit_status = 0;
		return ;
	}
	if (!data->com->next && is_builtin(data->com->args[0]))
		execute_builtin_parent(data->com, data);
	else if (data->com->next)
		execute_pipeline(data);
	else
		execute_single_external_command(data->com, data);
}

void	execute_single_external_command(t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		data->last_exit_status = 0;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		data->last_exit_status = EXIT_GENERAL_ERROR;
		return ;
	}
	if (pid == 0)
		execute_command_in_child(cmd, data);
	else
	{
		handle_parent_wait(pid, 1, data);
		if (cmd->in_type == T_HEREDOC && cmd->in_file)
		{
			if (access(cmd->in_file, F_OK) == 0)
				unlink(cmd->in_file);
		}
	}
}
