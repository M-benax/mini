/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:46:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 19:35:50 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void set_env_var(t_env **env_list, char *var_name, char *var_value, t_data *data)
{
	t_env *current = *env_list;
	t_env *prev = NULL;

	while (current) {
		if (current->var && ft_strcmp(current->var, var_name) == 0) {
			gc_free_ptr(current->val, data);
			current->val = ft_strdup(var_value, data);
			return;
		}
		prev = current;
		current = current->next;
	}
	current = new_env_node(var_name, var_value, data);
	if (!current) return;
	if (!prev)
		*env_list = current;
	else
		prev->next = current;
}

int builtin_cd(char **args, t_env **env_list, t_data *data)
{
	char *target_path;
	char old_pwd_buffer[PATH_MAX];
	char new_pwd_buffer[PATH_MAX];
	char *path_from_env = NULL;
	int ret_status = 0;

	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		target_path = get_env_value(*env_list, "HOME", data);
		if (!target_path || target_path[0] == '\0') {
			display_error_message("cd", NULL, "HOME not set");
			gc_free_ptr(target_path, data);
			return (1);
		}
		path_from_env = target_path;
	} else if (args[2]) {
		display_error_message("cd", NULL, "too many arguments");
		return (1);
	} else
		target_path = args[1];

	old_pwd_buffer[0] = '\0';
	if (getcwd(old_pwd_buffer, PATH_MAX) == NULL) {
		display_error_message("cd", "getcwd for OLDPWD", strerror(errno));
	}

	if (chdir(target_path) != 0) {
		display_error_message("cd", target_path, strerror(errno));
		ret_status = 1;
	} else {
		if (getcwd(new_pwd_buffer, PATH_MAX) == NULL) {
			display_error_message("cd", "getcwd for PWD", strerror(errno));
			ret_status = 1;
		} else {
			if (old_pwd_buffer[0] != '\0')
				set_env_var(env_list, "OLDPWD", old_pwd_buffer, data);
			set_env_var(env_list, "PWD", new_pwd_buffer, data);
		}
	}
	gc_free_ptr(path_from_env, data);
	return (ret_status);
}