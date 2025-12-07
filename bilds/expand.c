/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:52:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/03 21:03:01 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	replace_arg(t_cmd *cmd, int *i_ptr, char **split, t_data *data)
{
	int	split_count;

	split_count = count_args(split);
	replace_arg_at(cmd, *i_ptr, split, data);
	if (split_count > 1)
		*i_ptr += split_count - 1;
	else if (split_count == 0)
		(*i_ptr)--;
}

static int	is_expandable_arg(char *arg)
{
	return (arg && ft_strchr(arg, '$') && ft_strcmp(arg, "$") != 0);
}

static void	process_arg_expansion(t_data *data, int *i)
{
	int	prev_i;

	prev_i = *i;
	if (should_expand_full_arg(data->com->args[*i], data, *i))
		handle_full_arg_expansion(data, i);
	else
		handle_partial_arg_expansion(data, i);
	if (*i == prev_i)
		(*i)++;
}

static void	expand_args_one_cmd(t_data *data)
{
	int	i;

	i = 0;
	while (i >= 0 && i < data->com->argc && data->com->args[i])
	{
		if (should_skip_expansion(data, i)
			|| !is_expandable_arg(data->com->args[i]))
		{
			i++;
			continue ;
		}
		process_arg_expansion(data, &i);
	}
}

int	expand_all_commands(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->com;
	while (cmd)
	{
		expand_args_one_cmd(data);
		cmd = cmd->next;
	}
	return (0);
}
