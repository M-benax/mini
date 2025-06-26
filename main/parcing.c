/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:40:51 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 15:32:15 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int too_many_heredocs(char *line)
{
	int count = 0;
	int i = 0;

	while (line[i])
	{
		if (line[i] == '<' && line[i + 1] == '<')
		{
			count++;
			if (count > MAX_HEREDOCS)
			{
				return (1);
			}
			i++;
		}
		i++;
	}
	return (0);
}

static int syntax_quote_or_pipe(char *line,t_data *data)
{
	if(unclosed_quote(line))
	{
		printf("syntax error (unclosed quote)\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	if (error_pipe(line))
	{
		printf("syntax error near unexpected token `||'\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	if (too_many_heredocs(line))
	{
		printf("bash: maximum here-document count exceeded\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	return (1);
}

void parcer_and_exec(t_comand *token, t_data *data)
{
	parse_command(token, data);
	expand_all_commands(data);
	start_execution(data);
}
void parcing(t_data *data)
{
	char *line;
	t_comand *token;
	
	while (1)
	{
		signal_init();
		line = readline("minishell-$ ");
		if (!line)
		{
			printf("exit\n");
			rl_clear_history();
			gc_free_all(data);
		    exit(data->last_exit_status);
		}
		add_history(line);
		if (!syntax_quote_or_pipe(line, data))
			continue;
		token = tokens(line, data);
		if (!chech_syntax(token, line, data))
			continue;
		parcer_and_exec(token, data);
		free (line);
	}
}
