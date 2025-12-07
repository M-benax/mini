/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:40 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/06 20:24:37 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(char **args, t_env **env_list, t_data *data)
{
	char	*target_path;
	char	*path_from_env;
	int		result;

	path_from_env = NULL;
	if (args[1] && args[2])
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
