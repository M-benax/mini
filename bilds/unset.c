/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:01:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:24:31 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	remove_env_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->var, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

static int	is_valid_var_name(const char *str)
{
	int	i;

	i = 1;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	unset(char **args, t_env **env)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			printf("minishell: unset: `%s`: not a valid identifier\n", args[i]);
			status = EXIT_GENERAL_ERROR;
			i++;
			continue ;
		}
		remove_env_var(env, args[i]);
		i++;
	}
	return (status);
}
