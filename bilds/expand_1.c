/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:13 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/26 16:53:52 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	should_expand_full_arg(char *arg, t_data *data, int i)
{
	char	*eq;
	char	*dlr;

	eq = ft_strchr(arg, '=');
	dlr = ft_strchr(arg, '$');
	if (!dlr)
		return (0);
	if (eq && dlr > eq)
		return (1);
	if (data->com->quoted_flags[i] == 2)
		return (1);
	return (0);
}

char	*expand_env_variable(char *res, const char *str, int *i, t_data *data)
{
	int		j;
	char	*var_name;
	char	*value;
	char	*tmp;
	size_t	result_len;

	j = *i + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_strndup(str + *i + 1, j - *i - 1, data);
	value = get_env_value(data->tenv, var_name, data);
	if (!value)
		value = "";
	result_len = ft_strlen(res);
	tmp = gc_malloc(result_len + ft_strlen(value) + 1, data);
	ft_strcpy(tmp, res);
	ft_strcpy(tmp + result_len, value);
	gc_free_ptr(res, data);
	gc_free_ptr(var_name, data);
	*i = j;
	return (tmp);
}

char	*append_char_to_result(char *result, char c, t_data *data)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(result);
	tmp = gc_malloc(len + 2, data);
	ft_strcpy(tmp, result);
	tmp[len] = c;
	tmp[len + 1] = '\0';
	gc_free_ptr(result, data);
	return (tmp);
}

int	handle_dollar(const char *s, char **res, int *i, t_data *d)
{
	if (s[*i + 1] == '?')
	{
		*res = ft_itoa(d->last_exit_status, d);
		*i += 2;
	}
	else if (ft_isalpha(s[*i + 1]) || s[*i + 1] == '_' || s[*i + 1] == '$')
		*res = expand_env_variable(*res, s, i, d);
	else
	{
		*res = append_char_to_result(*res, '$', d);
		(*i)++;
	}
	return (0);
}
