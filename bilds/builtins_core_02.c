/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core_02.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:10:59 by elben-id          #+#    #+#             */
/*   Updated: 2025/06/27 14:11:01 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gc_free_array(char **array, t_data *data)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		gc_free_ptr(array[i], data);
		i++;
	}
	gc_free_ptr(array, data);
}
