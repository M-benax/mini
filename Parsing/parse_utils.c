/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/08 16:47:03 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_redirection_filename(char *filename, t_data *data)
{
	char	*expanded;
	char	**words;
	int		word_count;

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

static void	handle_redirection_error(t_cmd *cmd,
		t_comand *token, int redir_type)
{
	display_error_message("minishell", token->next->word,
		"ambiguous redirect");
	if (redir_type == T_REDIR_IN)
	{
		cmd->in_file = NULL;
		cmd->has_redirect_error = 1;
	}
	else
	{
		cmd->out_file = NULL;
		cmd->has_redirect_error = 1;
	}
}

static	void	handle_input_redirection(t_cmd *cmd, char *filename,
					int redir_type, t_data *data)
{
	if (access(filename, F_OK) != 0)
	{
		display_error_message(NULL, filename, "No such file or directory");
		cmd->has_redirect_error = 1;
		gc_free_ptr(filename, data);
		return ;
	}
	if (cmd->in_file)
		gc_free_ptr(cmd->in_file, data);
	cmd->in_file = filename;
	cmd->in_type = redir_type;
}

static	void	handle_output_redirection(t_cmd *cmd, char *filename,
					int redir_type, t_data *data)
{
	if (cmd->out_file)
		gc_free_ptr(cmd->out_file, data);
	cmd->out_file = filename;
	cmd->out_type = redir_type;
}

void	handle_redirection(t_cmd *cmd, t_comand *token,
			t_data *data, int redir_type)
{
	char	*expanded_filename;

	expanded_filename = expand_redirection_filename(token->next->word, data);
	if (!expanded_filename)
	{
		handle_redirection_error(cmd, token, redir_type);
		return ;
	}
	if (redir_type == T_REDIR_IN)
		handle_input_redirection(cmd, expanded_filename, redir_type, data);
	else
		handle_output_redirection(cmd, expanded_filename, redir_type, data);
}
