/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_04.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:35:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/08 16:58:19 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	start_execution(t_data *data)
{
	if (!data || !data->com)
		return ;
	if (data->com->has_redirect_error)
	{
		data->last_exit_status = EXIT_GENERAL_ERROR;
		return ;
	}
	if (data->com->args && data->com->args[0] && data->com->args[0][0] == '\0')
	{
		display_error_message("", NULL, "command not found");
		data->last_exit_status = EXIT_CMD_NOT_FOUND;
		return ;
	}
	if (!data->com->next && data->com->args && is_builtin(data->com->args[0]))
		execute_builtin_parent(data->com, data);
	else if (data->com->next)
		execute_pipeline(data);
	else
		execute_single_external_command(data->com, data);
}

int	is_minishell_command(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strcmp(cmd_name, "./minishell") == 0
		|| ft_strcmp(cmd_name, "minishell") == 0);
}
