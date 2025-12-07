/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_h.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:39:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/26 17:28:42 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_arg(t_cmd *cmd, char *word, int q, t_data *data)
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

void	put_command_part(t_comand *token, t_cmd *cmd, t_data *data)
{
	if (token->type == T_WORD)
		add_arg(cmd, token->word, token->quoted, data);
	else if (token->type == T_REDIR_IN)
		handle_redirection(cmd, token, data, T_REDIR_IN);
	else if (token->type == T_REDIR_OUT)
		handle_redirection(cmd, token, data, T_REDIR_OUT);
	else if (token->type == T_APPEND)
		handle_redirection(cmd, token, data, T_APPEND);
}

int	parse_heredoc_if_needed(t_comand **tokens,
		t_cmd *node, t_data *data)
{
	char	*limiter;

	if ((*tokens)->type == T_HEREDOC)
	{
		if (!(*tokens)->next)
		{
			printf("syntax error: missing heredoc limiter\n");
			return (1);
		}
		limiter = (*tokens)->next->word;
		node->q_limeter = (*tokens)->next->quoted;
		if (handel_heredoc(limiter, node, data) == 1)
		{
			write(STDOUT_FILENO, "> ^C\n", 5);
			return (1);
		}
		node->in_type = T_HEREDOC;
		*tokens = (*tokens)->next->next;
		return (2);
	}
	return (0);
}

t_cmd	*init_node(t_data *data)
{
	t_cmd	*node;

	node = gc_malloc(sizeof(t_cmd), data);
	node->args = NULL;
	node->quoted_flags = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->in_type = 0;
	node->out_type = 0;
	node->has_redirect_error = 0;
	node->next = NULL;
	node->argc = 0;
	node->q_limeter = 0;
	return (node);
}
