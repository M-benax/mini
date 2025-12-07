/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:01:21 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/13 00:39:52 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	len(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static int	seg(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

char	*ft_itoa(int n, t_data *data)
{
	unsigned int	nb;
	int				lent;
	char			*res;

	lent = len(n);
	res = (char *)gc_malloc(sizeof(char) * (lent + 1), data);
	res[lent] = '\0';
	nb = seg(n);
	if (nb == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (n < 0)
		res[0] = '-';
	while (nb > 0)
	{
		res[--lent] = (nb % 10) + '0';
		nb /= 10;
	}
	return (res);
}
