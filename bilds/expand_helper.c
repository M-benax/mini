/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/13 19:59:33 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	get_var_name(const char *line, size_t *i, char *var_name)
{
	size_t	k;

	k = 0;
	(*i)++;
	while (is_var_char(line[*i]) && k < 255)
		var_name[k++] = line[(*i)++];
	var_name[k] = '\0';
	return (k);
}

static size_t	handle_variable(const char *line, size_t *j, t_data *data)
{
	size_t	k;
	char	var_name[256];
	char	*val;

	k = 0;
	(*j)++;
	while (is_var_char(line[*j]) && k < 255)
		var_name[k++] = line[(*j)++];
	var_name[k] = '\0';
	val = get_env_value(data->tenv, var_name, data);
	if (val)
		return (ft_strlen(val));
	return (0);
}

static size_t	handle_normal_char(size_t *j)
{
	(*j)++;
	return (1);
}

size_t	calc_var_len(const char *line, size_t *i, t_data *data)
{
	size_t	total;
	size_t	j;

	j = 0;
	total = 0;
	(void)i;
	while (line[j])
	{
		if (line[j] == '$' && is_var_start(line[j + 1]))
			total += handle_variable(line, &j, data);
		else
			total += handle_normal_char(&j);
	}
	return (total);
}
