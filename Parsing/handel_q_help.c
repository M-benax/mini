/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_q_help.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:24:53 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/13 00:34:27 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_escape(char *token, char *new_token, t_quote_ctx *ctx)
{
	if (token[ctx->i + 1])
		new_token[ctx->j++] = token[++ctx->i];
	ctx->i++;
}

int	is_escape(char *token, int i, char quote)
{
	return (token[i] == '\\' && (quote == 0 || quote == '"'));
}

int	is_dollar_quote(char *token, int i)
{
	return (token[i] == '$' && token[i + 1] == '"');
}

int	is_opening_quote(char c, char quote)
{
	return ((c == '\'' || c == '"') && quote == 0);
}

void	handle_closing_quote(t_quote_ctx *ctx)
{
	ctx->quote = 0;
	ctx->i++;
}
