/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memrory_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:59:53 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:01:19 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1, t_data *data)
{
	char	*p;
	size_t	i;

	if (!s1)
		return (NULL);
	i = ft_strlen(s1);
	p = (char *)gc_malloc((i + 1) * sizeof(char), data);
	ft_memcpy(p, s1, i + 1);
	return (p);
}

t_comand	*new_node(char *token, int q, t_data *data)
{
	t_comand	*node;

	node = (t_comand *)gc_malloc(sizeof(t_comand), data);
	node->word = ft_strdup(token, data);
	node->quoted = q;
	put_type(node);
	node->next = NULL;
	return (node);
}

char	*ft_strndup(const char *s1, size_t size, t_data *data)
{
	char	*p;

	p = (char *)gc_malloc((size + 1) * sizeof(char), data);
	ft_memcpy(p, s1, size);
	p[size] = '\0';
	return (p);
}
