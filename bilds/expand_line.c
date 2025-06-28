/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:16:50 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/28 11:01:49 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static	int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

size_t	c_v_value(const char *l, size_t *i, char *res, size_t j, t_data *data)
{
	char	var_name[256];
	size_t	k;
	char	*value;
	size_t	len;

	k = 0;
	(*i)++;
	while (is_var_char(l[*i]) && k < sizeof(var_name) - 1)
	{
		var_name[k++] = l[*i];
		(*i)++;
	}
	var_name[k] = '\0';
	value = get_env_value(data->tenv, var_name, data);
	if (value)
	{
		len = strlen(value);
		ft_memcpy(res + j, value, len);
		return (j + len);
	}
	return (j);
}

static size_t calculate_expanded_length(const char *line, t_data *data)
{
	size_t i = 0;
	size_t total = 0;
	while (line[i])
	{
		if (line[i] == '$' && is_var_start(line[i + 1]))
		{
			char var_name[256];
			size_t k = 0;
			i++;
			while (is_var_char(line[i]) && k < sizeof(var_name) - 1)
				var_name[k++] = line[i++];
			var_name[k] = '\0';

			char *val = get_env_value(data->tenv, var_name, data);
			total += val ? ft_strlen(val) : 0;
		}
		else
		{
			total++;
			i++;
		}
	}
	return total;
}

char	*expand_line(const char *line, t_data *data)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	len = calculate_expanded_length(line, data);
	result = gc_malloc(len * 2 + 1, data);
	if (!result)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$' && is_var_start(line[i + 1]))
			j = c_v_value(line, &i, result, j, data);
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}
