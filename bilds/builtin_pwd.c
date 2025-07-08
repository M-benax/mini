/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:54 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/29 17:59:22 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_env *env_list, t_data *data)
{
	char	*pwd_val;
	char	current_path_buffer[PATH_MAX];

	pwd_val = get_env_value(env_list, "PWD", data);
	if (pwd_val && pwd_val[0] != '\0')
	{
		write(STDOUT_FILENO, pwd_val, ft_strlen(pwd_val));
		write(STDOUT_FILENO, "\n", 1);
		gc_free_ptr(pwd_val, data);
		return (0);
	}
	gc_free_ptr(pwd_val, data);
	if (getcwd(current_path_buffer, PATH_MAX) == NULL)
	{
		display_error_message("pwd", NULL, strerror(errno));
		return (1);
	}
	write(STDOUT_FILENO, current_path_buffer, ft_strlen(current_path_buffer));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
