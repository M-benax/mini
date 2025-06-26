/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 23:25:36 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 19:07:11 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error_pipe(char *line)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	quote = '\0';
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote == '\0')
				quote = line[i];
			else if (quote == line[i])
				quote = '\0';
		}
		else if (line[i] == '|' && quote == '\0')
		{
			j = i + 1;
			while (line[j] && is_space(line[j]))
				j++;

			if (line[j] == '|' || line[j] == '\0')
				return (1);
		}
		i++;
	}
	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	if (line[i] == '|' && quote == '\0')
		return (1);
	i = ft_strlen(line) - 1;
	while (i >= 0 && is_space(line[i]))
		i--;
	if (i >= 0 && line[i] == '|' && quote == '\0')
		return (1);

	return (0);
}

int	chech_syntax(t_comand *check, char *line, t_data *data)
{
	if (!check)
		return(0);
	if (check_syntax(check))
	{
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	return (1);
}

int	unclosed_quote(const char *str)
{
	int i;
	char current_quote;

	i = 0;
	current_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i - 1] != '\\'))
		{
			if (!current_quote)
				current_quote = str[i];
			else if (current_quote == str[i])
				current_quote = 0;
		}
		i++;
	}
	return (current_quote != 0);
}

int	check_syntax(t_comand *tokens)
{
	t_comand	*tmp;

	tmp = tokens;
	if(tmp->type == T_PIPE)
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


