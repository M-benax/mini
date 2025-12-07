/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:06:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/21 18:42:56 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*p_sr;
	unsigned char		*p_dst;

	if (dst == NULL || src == NULL)
		return (NULL);
	if (dst == src)
		return (dst);
	p_sr = (const unsigned char *)src;
	p_dst = (unsigned char *) dst;
	while (n--)
		*p_dst++ = *p_sr++;
	return (dst);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	c;

	c = 0;
	while (s1[c] == s2[c] && s1[c] != '\0' && s2[c] != '\0')
		c++;
	return (s1[c] - s2[c]);
}

char	*ft_strjoin(char const *s1, char const *s2, t_data *data)
{
	char	*p;
	size_t	i1;
	size_t	i2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return ((char *)s2);
	if (s1 && !s2)
		return ((char *)s1);
	i1 = ft_strlen(s1);
	i2 = ft_strlen(s2);
	p = (char *)gc_malloc((i1 + i2 + 1) * sizeof (char), data);
	ft_memcpy(p, s1, i1);
	ft_memcpy(p + i1, s2, i2);
	p[i1 + i2] = '\0';
	return (p);
}
