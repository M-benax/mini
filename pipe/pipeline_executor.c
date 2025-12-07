/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:39:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/11 12:29:01 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipeline(t_data *data)
{
	int			input_fd;
	t_pid_list	*pid_list_head;
	pid_t		last_pid_in_pipeline;

	g_sigint_received = 0;
	input_fd = STDIN_FILENO;
	pid_list_head = process_pipeline_cmds(data,
			&input_fd, &last_pid_in_pipeline);
	if (pid_list_head)
		pipeline_cleanup(pid_list_head, last_pid_in_pipeline, data);
}
