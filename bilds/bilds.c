/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilds.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:46:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/16 18:22:22 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	char	**extract_vars(t_env *env, t_data *data)
{
	int			i;
	char		**arr;
	int			size;

	i = 0;
	size = count_nodes(env);
	arr = gc_malloc(sizeof(char *) * (size + 1), data);
	while (env)
	{
		arr[i++] = env->var;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

static	void	sort_vars(char **vars)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	while (vars[i])
	{
		j = i + 1;
		while (vars[j])
		{
			if (ft_strcmp(vars[i], vars[j]) > 0)
			{
				tmp = vars[i];
				vars[i] = vars[j];
				vars[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static	t_env	*find_var(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strcmp(env->var, var) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	print_export(t_env *env, t_data *data)
{
	int		i;
	char	**vars;
	t_env	*node;

	vars = extract_vars(env, data);
	sort_vars(vars);
	i = 0;
	while (vars[i])
	{
		if (ft_strcmp(vars[i], "s_pwd") == 0)
		{
			i++;
			continue ;
		}
		node = find_var(env, vars[i]);
		if (node->val)
			printf("declare -x %s=\"%s\"\n", node->var, node->val);
		else
			printf("declare -x %s\n", node->var);
		i++;
	}
	gc_free_ptr(vars, data);
}
