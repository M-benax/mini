/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 23:25:36 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/13 01:05:19 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	chech_syntax(t_comand *check, char *line, t_data *data)
{
	if (!check)
	{
		free(line);
		return (0);
	}
	if (check_syntax(check))
	{
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	return (1);
}

int	check_syntax(t_comand *tokens)
{
	t_comand	*tmp;

	tmp = tokens;
	if (tmp->type == T_PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	while (tmp)
	{
		if (tmp->type >= T_REDIR_IN && tmp->type <= T_HEREDOC)
		{
			if (!tmp->next || tmp->next->type != T_WORD)
			{
				printf(" syntax error near unexpected token (redirection)\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
