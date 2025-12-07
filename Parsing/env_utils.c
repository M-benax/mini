/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/14 02:23:46 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var_plus_equal(char *arg, t_data *data)
{
	int		i;
	char	*res;
	int		j;

	i = 0;
	while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
		i++;
	res = gc_malloc(i + 1, data);
	if (!res)
		return (NULL);
	j = 0;
	while (j < i)
	{
		res[j] = arg[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*get_val_plus_equal(char *arg, t_data *data)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
			return (ft_strdup(arg + i + 2, data));
		if (arg[i] == '=')
			return (ft_strdup(arg + i + 1, data));
		i++;
	}
	return (NULL);
}

static t_env	*init_default_env(t_data *data)
{
	t_env	*node;
	char	*str;

	str = getcwd(NULL, 0);
	node = new_env_node("pwd", str, data);
	free(str);
	node->next = new_env_node("s_pwd", str, data);
	node->next->next = new_env_node("PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
			data);
	node->next->next->next = new_env_node("SHLVL", "1", data);
	node->next->next->next->next = new_env_node("_", "/usr/bin/env", data);
	node->next->flage = 0;
	return (node);
}

t_env	*init_env(char **envp, t_data *data)
{
	if (!envp || !envp[0])
		return (init_default_env(data));
	return (parse_envp(envp, data));
}
