/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/13 01:21:27 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(const char *s, unsigned int start, size_t len, t_data *data)
{
	char	*substr;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (gc_malloc(1, data));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	substr = gc_malloc(len + 1, data);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

static char	*extract_var_name_from_rhs(char *rhs, int *out_i, t_data *data)
{
	int	i;

	i = 2;
	while (rhs[i] && (ft_isalnum(rhs[i]) || rhs[i] == '_'))
		i++;
	*out_i = i;
	return (ft_substr(rhs, 2, i - 2, data));
}

static char	*build_expanded_token(t_tok_parts *parts, char *var_name,
	int i, t_data *data)
{
	char	*var_value;
	char	*rest;
	char	*new_token;

	var_value = get_env_value(data->tenv, var_name, data);
	if (!var_value)
		var_value = "";
	rest = ft_strdup(parts->rhs + i, data);
	new_token = gc_malloc(ft_strlen(parts->lhs) + 1
			+ ft_strlen(var_value) + ft_strlen(rest) + 1, data);
	if (!new_token)
		return (NULL);
	ft_strcpy(new_token, parts->lhs);
	ft_strcat(new_token, "=");
	ft_strcat(new_token, var_value);
	ft_strcat(new_token, rest);
	return (new_token);
}

char	*handel_tok(char *token, t_data *data)
{
	char		*eq;
	char		*var_name;
	char		*new_token;
	int			i;
	t_tok_parts	parts;

	eq = ft_strchr(token, '=');
	if (!eq)
		return (token);
	parts.lhs = ft_strndup(token, eq - token, data);
	parts.rhs = ft_strdup(eq + 1, data);
	if (parts.rhs[0] == '"' && parts.rhs[1] == '$')
	{
		var_name = extract_var_name_from_rhs(parts.rhs, &i, data);
		new_token = build_expanded_token(&parts, var_name, i, data);
		if (!new_token)
			return (token);
		gc_free_ptr(token, data);
		return (new_token);
	}
	return (token);
}
