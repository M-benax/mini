/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:03:39 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/28 12:08:14 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char    *expand_redirection_filename(char *filename, t_data *data)
{
    char    *expanded;
    char    **words;
    int        word_count;

    expanded = expand_var(filename, data);
    if (!expanded || !*expanded)
        return (NULL);
    words = ft_split(expanded, ' ', data);
    word_count = 0;
    while (words && words[word_count])
        word_count++;
    if (word_count != 1)
        return (NULL);
    return (expanded);
}

// static int    open_output_file(char *filename, int type, t_data *data)
// {
//     char    *expanded_name;
//     int        flags;
//     int        fd;

//     expanded_name = expand_redirection_filename(filename, data);
//     if (!expanded_name)
//     {
//         display_error_message("minishell", filename, "ambiguous redirect");
//         return (-1);
//     }
//     if (type == T_REDIR_OUT)
//         flags = O_WRONLY | O_CREAT | O_TRUNC;
//     else
//         flags = O_WRONLY | O_CREAT | O_APPEND;
//     fd = open(expanded_name, flags, 0644);
//     if (fd == -1)
//         display_error_message(NULL, expanded_name, strerror(errno));
//     return (fd);
// }

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
	char *expanded_filename;

	if (token->type == T_WORD)
	{
		add_arg(cmd, token->word, token->quoted, data);
	}
	else if (token->type == T_REDIR_IN)
	{
		cmd->in_type = T_REDIR_IN;
		expanded_filename = expand_redirection_filename(token->next->word, data);
		if (!expanded_filename)
		{
			display_error_message("minishell", token->next->word, "ambiguous redirect");
			cmd->in_file = NULL;
			cmd->has_redirect_error = 1;
		}
		else
			cmd->in_file = expanded_filename;
	}
	else if (token->type == T_REDIR_OUT)
	{
		cmd->out_type = T_REDIR_OUT;
		expanded_filename = expand_redirection_filename(token->next->word, data);
		if (!expanded_filename)
		{
			display_error_message("minishell", token->next->word, "ambiguous redirect");
			cmd->out_file = NULL;
			cmd->has_redirect_error = 1;
		}
		else
			cmd->out_file = expanded_filename;
	}
	else if (token->type == T_APPEND)
	{
		cmd->out_type = T_APPEND;
		expanded_filename = expand_redirection_filename(token->next->word, data);
		if (!expanded_filename)
		{
			display_error_message("minishell", token->next->word, "ambiguous redirect");
			cmd->out_file = NULL;
			cmd->has_redirect_error = 1;
		}
		else
			cmd->out_file = expanded_filename;
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
        else if ((*tokens)->type == T_REDIR_IN
              || (*tokens)->type == T_REDIR_OUT
              || (*tokens)->type == T_APPEND)
        {
            put_command_part(*tokens, node, data);
            // Skip the redirection token + the filename token
            if ((*tokens)->next)
                *tokens = (*tokens)->next->next;
            else
                *tokens = NULL; // Defensive: no next token for filename
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
