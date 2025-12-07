/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:54 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/06 20:20:38 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_current_path_if_valid(void)
{
	char	current_path_buffer[PATH_MAX];

	if (getcwd(current_path_buffer, PATH_MAX) != NULL)
	{
		write(STDOUT_FILENO, current_path_buffer,
			ft_strlen(current_path_buffer));
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

static int	print_env_pwd_if_valid(t_env *env_list, t_data *data)
{
	char	*pwd_val;

	pwd_val = get_env_value(env_list, "PWD", data);
	if (pwd_val && pwd_val[0] != '\0')
	{
		write(STDOUT_FILENO, pwd_val, ft_strlen(pwd_val));
		write(STDOUT_FILENO, "\n", 1);
		gc_free_ptr(pwd_val, data);
		return (1);
	}
	gc_free_ptr(pwd_val, data);
	return (0);
}

static int	print_s_pwd_if_valid(t_env *env_list, t_data *data)
{
	char	*pwd_val;

	pwd_val = get_env_value(env_list, "s_pwd", data);
	if (pwd_val && pwd_val[0] != '\0')
	{
		write(STDOUT_FILENO, pwd_val, ft_strlen(pwd_val));
		write(STDOUT_FILENO, "\n", 1);
		gc_free_ptr(pwd_val, data);
		return (1);
	}
	gc_free_ptr(pwd_val, data);
	return (0);
}

int	builtin_pwd(t_env *env_list, t_data *data)
{
	if (print_current_path_if_valid())
		return (0);
	if (print_env_pwd_if_valid(env_list, data))
		return (0);
	if (print_s_pwd_if_valid(env_list, data))
		return (0);
	display_error_message("pwd", NULL, strerror(errno));
	return (1);
}
