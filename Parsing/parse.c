/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:03:39 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 19:07:25 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void add_arg(t_cmd *cmd, char *word, int q, t_data *data)
{
	int		i;
	char	**new_args;
	int		*new_quoted_flags;

	i = 0;
	
	while (cmd->args && cmd->args[i])
		i++;
	new_args = gc_malloc(sizeof(char *) * (i + 2), data);
	new_quoted_flags = gc_malloc(sizeof(int) * (i + 1), data);
	copy_args(cmd->args, new_args, i);
	copy_flags(cmd->quoted_flags, new_quoted_flags, i);
	new_args[i] = ft_strdup(word, data);
	new_args[i + 1] = NULL;
	new_quoted_flags[i] = q;
	cmd->args = new_args;
	cmd->quoted_flags = new_quoted_flags;
	cmd->argc = i + 1;

}

static void put_command_part(t_comand *token, t_cmd *cmd, t_data *data)
{
	if (token->type == T_WORD)
		add_arg(cmd, token->word, token->quoted, data);
	else if (token->type == T_REDIR_IN)
	{
		cmd->in_type = T_REDIR_IN;
		cmd->in_file = ft_strdup(token->next->word, data);
	}
	else if (token->type == T_REDIR_OUT)
	{
		cmd->out_type = T_REDIR_OUT;
		cmd->out_file = ft_strdup(token->next->word, data);
	}
	else if (token->type == T_APPEND)
	{
		cmd->out_type = T_APPEND;
		cmd->out_file = ft_strdup(token->next->word, data);
	}
}

static t_cmd *init_node(t_data *data)
{
	t_cmd *node;

	node = gc_malloc(sizeof(t_cmd), data);
	node->args = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->in_type = 0;
	node->out_type = 0;
	node->next = NULL;
	node->argc = 0;
	return (node);
}
static t_cmd *parse_single_command(t_comand **tokens, t_data *data)
{
	t_cmd *node;
	char *limiter;

	node = init_node(data);
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_HEREDOC)
		{
			if (!(*tokens)->next)
			{
				printf("syntax error: missing heredoc limiter\n");
				return (NULL);
			}
			limiter = (*tokens)->next->word;
			handel_heredoc(limiter, node, data);
			node->in_type = T_HEREDOC;
			*tokens = (*tokens)->next->next;
		}
		else
		{
			put_command_part(*tokens, node, data);
			*tokens = (*tokens)->next;
		}
	}
	return (node);
}

void parse_command(t_comand *tokens, t_data *data)
{
	t_cmd *head = NULL;
	t_cmd *curent = NULL;
	t_cmd *node;

	while (tokens)
	{
		node = parse_single_command(&tokens, data);
		if (!node)
			return;

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
