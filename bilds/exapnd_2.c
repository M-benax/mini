/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exapnd_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:37 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/26 16:49:26 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_arg(char *arg, t_data *data)
{
	char	*expanded;

	expanded = expand_var(arg, data);
	gc_free_ptr(arg, data);
	return (expanded);
}

int	should_skip_expansion(t_data *data, int i)
{
	return (data->com->quoted_flags[i] == 1);
}

char	*expand_var(const char *str, t_data *data)
{
	int		i;
	char	*result;
	int		y;

	i = 0;
	result = gc_malloc(1, data);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '$')
			y = handle_dollar(str, &result, &i, data);
		else
		{
			result = append_char_to_result(result, str[i], data);
			i++;
		}
	}
	return (result);
}

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

void	replace_arg_at(t_cmd *cmd, int index, char **split, t_data *data)
{
	t_replace_ctx	ctx;
	int				split_count;
	int				new_argc;
	char			**new_args;
	int				*new_flags;

	ctx.cmd = cmd;
	ctx.index = index;
	ctx.split = split;
	ctx.data = data;
	split_count = count_args(split);
	new_argc = cmd->argc - 1 + split_count;
	new_args = gc_malloc(sizeof(char *) * (new_argc + 1), data);
	new_flags = gc_malloc(sizeof(int) * new_argc, data);
	copy_before_index(&ctx, new_args, new_flags);
	copy_split(&ctx, new_args, new_flags, index);
	copy_after_index(&ctx, new_args, new_flags, index + split_count);
	new_args[new_argc] = NULL;
	gc_free_ptr(cmd->args[index], data);
	gc_free_ptr(cmd->args, data);
	gc_free_ptr(cmd->quoted_flags, data);
	cmd->args = new_args;
	cmd->quoted_flags = new_flags;
	cmd->argc = new_argc;
}
