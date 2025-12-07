/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 03:18:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/07 18:52:25 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	_sigint_handler(int sig)
{
	(void)sig;
	if (SIGINT && (g_sigint_received != 555
			&& g_sigint_received != 600
			&& g_sigint_received != 700))
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sigint_received = 1;
	}
}

void	signal_init(void)
{
	signal(SIGINT, _sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_init1(void)
{
	signal(SIGINT, _sigint_handler);
	signal(SIGQUIT, SIG_DFL);
}
