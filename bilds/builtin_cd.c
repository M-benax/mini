/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:40 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/29 18:23:27 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_env_var(t_env **env_list, char *var_name, char *var_value,
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

static char	*get_target_path(char **args, t_env *env_list, t_data *data)
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

static int	change_directory(char *target_path, t_env **env_list, t_data *data)
{
	char	old_pwd_buffer[PATH_MAX];
	char	new_pwd_buffer[PATH_MAX];

	old_pwd_buffer[0] = '\0';
	if (getcwd(old_pwd_buffer, PATH_MAX) == NULL)
		display_error_message("cd", "getcwd for OLDPWD", strerror(errno));
	if (chdir(target_path) != 0)
	{
		display_error_message("cd", target_path, strerror(errno));
		return (1);
	}
	if (getcwd(new_pwd_buffer, PATH_MAX) == NULL)
	{
		display_error_message("cd", "getcwd for PWD", strerror(errno));
		return (1);
	}
	if (old_pwd_buffer[0] != '\0')
		set_env_var(env_list, "OLDPWD", old_pwd_buffer, data);
	set_env_var(env_list, "PWD", new_pwd_buffer, data);
	return (0);
}

int	builtin_cd(char **args, t_env **env_list, t_data *data)
{
	char	*target_path;
	char	*path_from_env;
	int		result;

	path_from_env = NULL;
	if (args[2])
	{
		display_error_message("cd", NULL, "too many arguments");
		return (1);
	}
	target_path = get_target_path(args, *env_list, data);
	if (!target_path)
		return (1);
	if (target_path != args[1])
		path_from_env = target_path;
	result = change_directory(target_path, env_list, data);
	gc_free_ptr(path_from_env, data);
	return (result);
}
