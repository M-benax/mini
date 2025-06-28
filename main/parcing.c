/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:40:51 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/28 11:56:53 by aaboudra         ###   ########.fr       */
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
// void print_cmd(t_cmd *cmd)
// {
//     int i;

//     if (!cmd)
//     {
//         printf("Command is NULL\n");
//         return;
//     }

//     printf("Command details:\n");

//     // Print arguments
//     printf("  argc = %d\n", cmd->argc);
//     printf("  args = ");
//     for (i = 0; i < cmd->argc; i++)
//     {
//         printf("\"%s\"", cmd->args[i]);
//         if (i < cmd->argc - 1)
//             printf(", ");
//     }
//     printf("\n");

//     // Input redirection
//     printf("  in_type = %d", cmd->in_type);
//     if (cmd->in_file)
//         printf(" (file: \"%s\")", cmd->in_file);
//     printf("\n");

//     // Output redirection
//     printf("  out_type = %d", cmd->out_type);
//     if (cmd->out_file)
//         printf(" (file: \"%s\")", cmd->out_file);
//     printf("\n");

//     printf("----------------------------\n");
// }

// // إذا بغيت تطبع من data
// void print_data_cmd(t_data *data)
// {
//     if (!data || !data->com)
//     {
//         printf("No command in data\n");
//         return;
//     }

//     t_cmd *cmd_iter = data->com;
//     int cmd_num = 1;

//     while (cmd_iter)
//     {
//         printf("Command #%d:\n", cmd_num);
//         print_cmd(cmd_iter);
//         cmd_iter = cmd_iter->next;
//         cmd_num++;
//     }
// }
void parcer_and_exec(t_comand *token, t_data *data)
{
	parse_command(token, data);
	expand_all_commands(data);
	start_execution(data);
	//print_data_cmd(data);
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
