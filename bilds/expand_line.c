/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:16:50 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/04 08:56:19 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_args_one_cmd_1(t_cmd *cmd, char *epandd, t_data *data, int *i)
{
	char	**split;
	int		k;

	if (epandd == NULL)
		return ;
	split = ft_split(epandd, ' ', data);
	gc_free_ptr(epandd, data);
	replace_arg(cmd, i, split, data);
	k = 0;
	while (split[k] != NULL)
	{
		gc_free_ptr(split[k], data);
		k++;
	}
	gc_free_ptr(split, data);
}

int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*expand_line(const char *line, t_cmd *cmd, t_data *data)
{
	if (!line)
		return (NULL);
	if (cmd->q_limeter)
		return (ft_strdup(line, data));
	return (expand_var(line, data));
}
