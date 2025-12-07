/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:30:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/07 18:33:10 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_env_var(t_env **env_list, char *var_name, char *var_value,
		t_data *data)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, var_name) == 0)
		{
			gc_free_ptr(current->val, data);
			current->val = ft_strdup(var_value, data);
			return ;
		}
		prev = current;
		current = current->next;
	}
	current = new_env_node(var_name, var_value, data);
	if (!current)
		return ;
	if (!prev)
		*env_list = current;
	else
		prev->next = current;
}

char	*get_target_path(char **args, t_env *env_list, t_data *data)
{
	char	*target_path;

	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		target_path = get_env_value(env_list, "HOME", data);
		if (!target_path || target_path[0] == '\0')
		{
			display_error_message("cd", NULL, "HOME not set");
			gc_free_ptr(target_path, data);
			return (NULL);
		}
		return (target_path);
	}
	return (args[1]);
}

char	*construct_new_path(char *old_path, char *target, t_data *data)
{
	char	*new_path;
	char	*tmp;

	if (!old_path || !target)
		return (NULL);
	if (target[0] == '/')
		return (ft_strdup(target, data));
	if (ft_strcmp(target, "..") == 0)
	{
		tmp = ft_strjoin(old_path, "/", data);
		if (!tmp)
			return (NULL);
		new_path = ft_strjoin(tmp, "..", data);
		gc_free_ptr(tmp, data);
		return (new_path);
	}
	if (ft_strcmp(target, ".") == 0)
		return (ft_strdup(old_path, data));
	if (old_path[ft_strlen(old_path) - 1] == '/')
		return (ft_strjoin(old_path, target, data));
	tmp = ft_strjoin(old_path, "/", data);
	new_path = ft_strjoin(tmp, target, data);
	gc_free_ptr(tmp, data);
	return (new_path);
}

void	display_getcwd_error(char *arg, char *message)
{
	write(STDERR_FILENO, "cd: error retrieving current directory: ", 39);
	if (arg)
	{
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, message, ft_strlen(message));
	write(1, "\n", 1);
}
