/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bi2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:24:28 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:24:52 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57)
		|| (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

char    *get_env_value(t_env *env, char *key, t_data *data)
{
    while (env)
    {
        if (ft_strcmp(env->var, key) == 0)
            return (ft_strdup(env->val, data));
        env = env->next;
    }
    return (ft_strdup("", data));
}