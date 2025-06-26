/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 00:37:17 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:39:51 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	ft_isspace_local(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || \
        	 c == '\v' || c == '\f' || c == '\r');
}

int	ft_atoi(const char *str)
{
    int i;
    int sign;
    long result;

    i = 0;
    sign = 1;
    result = 0;
    while (str[i] && ft_isspace_local(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (result > (2147483647 - (str[i] - '0')) / 10 && sign == 1)
            return (-1); // Overflow (مثل atoi القياسية)
        if (result > (2147483648 - (str[i] - '0')) / 10 && sign == -1)
            return (0);  // Overflow (مثل atoi القياسية)
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return ((int)(result * sign));
}

int	is_s_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int	is_space(char s)
{
    return (s == ' ' || s == '\t' || s == '\n');
}




