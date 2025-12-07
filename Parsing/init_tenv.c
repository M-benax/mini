/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:48:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/03 21:53:39 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*new_env_node(char *var, char *val, t_data *data)
{
	t_env	*node;

	node = gc_malloc(sizeof(t_env), data);
	node->var = ft_strdup(var, data);
	if (!val)
		node->val = NULL;
	else
		node->val = ft_strdup(val, data);
	node->next = NULL;
	return (node);
}

t_env	*create_env_node(char **envp, int i, t_data *data)
{
	char	*var;
	char	*val;
	t_env	*node;

	var = get_var_plus_equal(envp[i], data);
	val = get_val_plus_equal(envp[i], data);
	node = new_env_node(var, val, data);
	gc_free_ptr(var, data);
	gc_free_ptr(val, data);
	return (node);
}

static t_env	*add_sec_pwd(t_env *envp, t_data *data)
{
	char	*str;
	t_env	*new_node;
	t_env	*tmp;

	str = getcwd(NULL, 0);
	if (!str)
		return (envp);
	new_node = new_env_node("s_pwd", str, data);
	free(str);
	tmp = envp;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (envp);
}

t_env	*parse_envp(char **envp, t_data *data)
{
	t_env	*head;
	t_env	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		if (!head)
		{
			head = create_env_node(envp, i, data);
			current = head;
		}
		else
		{
			current->next = create_env_node(envp, i, data);
			current = current->next;
		}
		i++;
	}
	return (add_sec_pwd(head, data));
}
