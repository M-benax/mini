/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:16:50 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 15:25:11 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static int is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static size_t copy_var_value(const char *line, size_t *i, char *result, size_t j, t_data *data)
{
	char var_name[256];
	size_t k;
	char *value;
	size_t len;

    k = 0;
	(*i)++;
	while (is_var_char(line[*i]) && k < sizeof(var_name) - 1)
	{
		var_name[k++] = line[*i];
		(*i)++;
	}
	var_name[k] = '\0';
	value = get_env_value(data->tenv, var_name, data);
	if (value)
	{
		len = strlen(value);
		memcpy(result + j, value, len);
		return (j + len);
	}
	return (j);
}

char *expand_line(const char *line, t_data *data)
{
	char *result;
	size_t i = 0;
	size_t j = 0;
	size_t len;

    len = ft_strlen(line);
	result = gc_malloc(len * 2 + 1, data);
	if (!result)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$' && is_var_start(line[i + 1]))
			j = copy_var_value(line, &i, result, j, data);
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}
