/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:03:39 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/07 22:14:28 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_redirection_if_needed(t_comand **tokens,
		t_cmd *node, t_data *data)
{
	if ((*tokens)->type == T_REDIR_IN
		|| (*tokens)->type == T_REDIR_OUT
		|| (*tokens)->type == T_APPEND)
	{
		put_command_part(*tokens, node, data);
		if ((*tokens)->next)
			*tokens = (*tokens)->next->next;
		else
			*tokens = NULL;
		return (1);
	}
	return (0);
}

static int	process_token(t_comand **tokens, t_cmd *node, t_data *data)
{
	int	status;

	status = parse_heredoc_if_needed(tokens, node, data);
	if (status == 1)
		return (1);
	else if (status == 2)
		return (2);
	if (parse_redirection_if_needed(tokens, node, data))
		return (3);
	put_command_part(*tokens, node, data);
	*tokens = (*tokens)->next;
	return (0);
}

static t_cmd	*parse_single_command(t_comand **tokens, t_data *data)
{
	t_cmd	*node;
	int		status;

	node = init_node(data);
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		status = process_token(tokens, node, data);
		if (status == 1)
			return (NULL);
		else if (status == 2 || status == 3)
			continue ;
	}
	return (node);
}

void	parse_command(t_comand *tokens, t_data *data)
{
	t_cmd	*head;
	t_cmd	*curent;
	t_cmd	*node;

	head = NULL;
	curent = NULL;
	while (tokens)
	{
		node = parse_single_command(&tokens, data);
		if (!node)
		{
			data->com = NULL;
			return ;
		}
		if (!head)
			head = node;
		else
			curent->next = node;
		curent = node;
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	data->com = head;
}
