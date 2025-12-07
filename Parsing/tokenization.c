/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:35:04 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/26 17:18:17 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*generate_tmp_filename(t_data *data)
{
	static int	i;
	char		*index_str;
	char		*file_name;

	i = 0;
	index_str = ft_itoa(i, data);
	file_name = ft_strjoin(ft_strdup("/tmp/heredoc_tmp_", data),
			index_str, data);
	while (!access(file_name, F_OK))
	{
		i++;
		index_str = ft_itoa(i, data);
		file_name = ft_strjoin(ft_strdup("/tmp/heredoc_tmp_", data),
				index_str, data);
	}
	gc_free_ptr(index_str, data);
	return (file_name);
}

int	open_file(char **filename, t_data *data)
{
	int	fd;

	*filename = generate_tmp_filename(data);
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		gc_free_all(data);
		exit (1);
	}
	return (fd);
}

static char	*collect_token(char *input, int *i, t_data *data)
{
	int		start;
	char	quote;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_s_char(input[*i]))
	{
		if ((input[*i] == '\'' || input[*i] == '\"')
			&& (*i == 0 || input[*i - 1] != '\\'))
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && (input[*i] != quote || input[*i - 1] == '\\'))
				(*i)++;
			if (input[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	return (ft_strndup(input + start, *i - start, data));
}

t_comand	*tokens_1(char *input, int *i, t_data *data)
{
	char	*token;
	int		start;

	while (input[*i] && is_space(input[*i]))
		(*i)++;
	if (input[*i] == '\0')
		return (NULL);
	if (is_s_char(input[*i]))
	{
		start = *i;
		(*i)++;
		if (input[start] == input[*i])
			(*i)++;
		token = ft_strndup(input + start, *i - start, data);
	}
	else
		token = collect_token(input, i, data);
	token = handel_tok(token, data);
	return (handle_quotes(token, data));
}

t_comand	*tokens(char *input, t_data *data)
{
	t_comand	*head;
	t_comand	*current;
	t_comand	*node;
	int			i;

	if (!input || !input[0])
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (input[i])
	{
		while (is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		node = tokens_1(input, &i, data);
		if (!head)
			head = node;
		else
			current->next = node;
		current = node;
	}
	return (head);
}
