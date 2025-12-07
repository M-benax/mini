/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_dir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:35:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/06 20:33:05 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	update_env_vars(t_cd_data *cd)
{
	if (cd->old_pwd[0] != '\0')
		set_env_var(cd->env_list, "OLDPWD", cd->old_pwd, cd->data);
	else if (cd->pwd_value)
		set_env_var(cd->env_list, "OLDPWD", cd->pwd_value, cd->data);
	return (0);
}

int	handle_getcwd_failure_internal(t_cd_data *cd)
{
	char	*new_pwd;

	display_getcwd_error("getcwd", "cannot access parent directories");
	if (!cd->pwd_value)
	{
		gc_free_ptr(cd->pwd_value, cd->data);
		return (0);
	}
	new_pwd = construct_new_path(cd->pwd_value, cd->target_path, cd->data);
	if (!new_pwd)
	{
		gc_free_ptr(cd->pwd_value, cd->data);
		return (0);
	}
	set_env_var(cd->env_list, "OLDPWD", cd->old_pwd, cd->data);
	set_env_var(cd->env_list, "PWD", new_pwd, cd->data);
	gc_free_ptr(new_pwd, cd->data);
	gc_free_ptr(cd->pwd_value, cd->data);
	return (0);
}

int	handle_getcwd_failure(char *pwd_value, char *target_path,
		char *old_pwd, t_data *data)
{
	t_cd_data	cd;
	t_env		**env_list;

	env_list = &data->tenv;
	cd.pwd_value = pwd_value;
	cd.target_path = target_path;
	cd.old_pwd = old_pwd;
	cd.env_list = env_list;
	cd.data = data;
	return (handle_getcwd_failure_internal(&cd));
}

static int	process_chdir(t_cd_data *cd)
{
	char	new_pwd_buffer[PATH_MAX];

	if (try_chdir_and_handle_failure(cd))
		return (1);
	if (!check_getcwd_status(cd, new_pwd_buffer))
		return (0);
	update_env_vars(cd);
	update_pwd_env_var(cd, new_pwd_buffer);
	gc_free_ptr(cd->pwd_value, cd->data);
	return (0);
}

int	change_directory(char *target_path, t_env **env_list, t_data *data)
{
	char		old_pwd_buffer[PATH_MAX];
	t_cd_data	cd;

	old_pwd_buffer[0] = '\0';
	cd.pwd_value = get_env_value(*env_list, "PWD", data);
	cd.old_pwd = old_pwd_buffer;
	cd.target_path = target_path;
	cd.env_list = env_list;
	cd.data = data;
	if (getcwd(old_pwd_buffer, PATH_MAX) == NULL)
		display_getcwd_error("getcwd", "cannot access parent directories");
	if (ft_strcmp(target_path, "..") == 0 && cd.pwd_value
		&& cd.pwd_value[0] != '\0')
	{
		if (!handle_parent_dir(cd.pwd_value, env_list, old_pwd_buffer, data))
			return (0);
	}
	return (process_chdir(&cd));
}
