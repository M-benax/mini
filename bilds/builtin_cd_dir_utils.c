/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_dir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:15:00 by elben-id          #+#    #+#             */
/*   Updated: 2025/08/08 16:54:32 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	try_chdir_and_handle_failure(t_cd_data *cd)
{
	if (chdir(cd->target_path) != 0)
	{
		display_error_message("cd", cd->target_path, strerror(errno));
		gc_free_ptr(cd->pwd_value, cd->data);
		return (1);
	}
	return (0);
}

int	check_getcwd_status(t_cd_data *cd, char *new_pwd_buffer)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (handle_getcwd_failure_internal(cd));
	if (getcwd(new_pwd_buffer, PATH_MAX) == NULL)
	{
		gc_free_ptr(cd->pwd_value, cd->data);
		return (0);
	}
	free(cwd);
	return (1);
}

void	update_pwd_env_var(t_cd_data *cd, char *new_pwd_buffer)
{
	char	*logical_pwd;

	if (cd->target_path[0] == '/')
		set_env_var(cd->env_list, "PWD", cd->target_path, cd->data);
	else if (cd->pwd_value)
	{
		logical_pwd = construct_new_path(cd->pwd_value,
				cd->target_path, cd->data);
		if (logical_pwd)
		{
			set_env_var(cd->env_list, "PWD", logical_pwd, cd->data);
			gc_free_ptr(logical_pwd, cd->data);
		}
		else
			set_env_var(cd->env_list, "PWD", new_pwd_buffer, cd->data);
	}
	else
		set_env_var(cd->env_list, "PWD", new_pwd_buffer, cd->data);
}

char	*extract_parent_dir(char *pwd_value, t_data *data)
{
	int		i;
	char	*parent_dir;

	i = ft_strlen(pwd_value) - 1;
	while (i > 0 && pwd_value[i] == '/')
		i--;
	while (i >= 0 && pwd_value[i] != '/')
		i--;
	if (i > 0)
		parent_dir = ft_strndup(pwd_value, i, data);
	else if (i == 0)
		parent_dir = ft_strdup("/", data);
	else
		parent_dir = NULL;
	return (parent_dir);
}

int	handle_parent_dir(char *pwd_value, t_env **env_list,
		char *old_pwd_buffer, t_data *data)
{
	char	*parent_dir;

	parent_dir = extract_parent_dir(pwd_value, data);
	if (parent_dir && chdir(parent_dir) == 0)
	{
		if (old_pwd_buffer[0] != '\0')
			set_env_var(env_list, "OLDPWD", old_pwd_buffer, data);
		else
			set_env_var(env_list, "OLDPWD", pwd_value, data);
		set_env_var(env_list, "PWD", parent_dir, data);
		gc_free_ptr(parent_dir, data);
		return (0);
	}
	if (parent_dir)
		gc_free_ptr(parent_dir, data);
	return (1);
}
