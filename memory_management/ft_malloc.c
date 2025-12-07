/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:11:20 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:01:01 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gc_free_pid_list(t_pid_list *list, t_data *data)
{
	t_pid_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		gc_free_ptr(tmp, data);
	}
}

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

void	gc_free_ptr(void *ptr, t_data *data)
{
	t_gc_node	*curr;
	t_gc_node	*prev;

	curr = data->g_gc_list;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				data->g_gc_list = curr->next;
			free(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	*gc_malloc(size_t size, t_data *data)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
	{
		gc_free_all(data);
		printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		gc_free_all(data);
		printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	node->ptr = ptr;
	node->next = data->g_gc_list;
	data->g_gc_list = node;
	return (ptr);
}

void	gc_free_all(t_data *data)
{
	t_gc_node	*tmp;

	while (data->g_gc_list)
	{
		tmp = data->g_gc_list;
		data->g_gc_list = data->g_gc_list->next;
		free(tmp->ptr);
		free(tmp);
	}
}
