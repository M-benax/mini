/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/29 20:00:01 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env_vars(t_env *env_list)
{
	t_env	*tmp;
	int		count;

	tmp = env_list;
	count = 0;
	while (tmp)
	{
		if (tmp->val)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_node, t_data *data)
{
	char	*var_eq_val;
	size_t	len_var;
	size_t	len_val;

	len_var = ft_strlen(env_node->var);
	len_val = ft_strlen(env_node->val);
	var_eq_val = gc_malloc((len_var + 1 + len_val + 1), data);
	if (!var_eq_val)
		return (NULL);
	ft_memcpy(var_eq_val, env_node->var, len_var);
	var_eq_val[len_var] = '=';
	ft_memcpy(var_eq_val + len_var + 1, env_node->val, len_val);
	var_eq_val[len_var + 1 + len_val] = '\0';
	return (var_eq_val);
}

static void	fill_env_array(t_env *env_list, char **env_array, t_data *data)
{
	t_env	*tmp;
	int		i;

	tmp = env_list;
	i = 0;
	while (tmp)
	{
		if (tmp->val)
		{
			env_array[i] = create_env_string(tmp, data);
			if (!env_array[i])
				return ;
			i++;
		}
		tmp = tmp->next;
	}
	env_array[i] = NULL;
}

char	**convert_env_list_to_array(t_env *env_list, t_data *data)
{
	int		count;
	char	**env_array;

	count = count_env_vars(env_list);
	env_array = gc_malloc(sizeof(char *) * (count + 1), data);
	if (!env_array)
		return (NULL);
	fill_env_array(env_list, env_array, data);
	return (env_array);
}
