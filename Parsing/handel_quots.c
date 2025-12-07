/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_quots.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 03:32:29 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/12 20:45:16 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_opening_quote(char c, t_quote_ctx *ctx)
{
	ctx->quote = c;
	if (c == '\'')
		ctx->quoted = 1;
	else
		ctx->quoted = 2;
	ctx->i++;
}

static void	handle_dollar_quote(char *token, char *new_token, t_quote_ctx *ctx)
{
	ctx->i += 2;
	ctx->quote = '"';
	ctx->quoted = 2;
	while (token[ctx->i] && !(token[ctx->i] == ctx->quote
			&& token[ctx->i - 1] != '\\'))
		new_token[ctx->j++] = token[ctx->i++];
	if (token[ctx->i] == ctx->quote)
		ctx->i++;
	ctx->quote = 0;
}

t_comand	*handle_quotes(char *token, t_data *data)
{
	t_quote_ctx	ctx;
	char		*new_token;

	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = 0;
	ctx.quoted = 0;
	new_token = gc_malloc(ft_strlen(token) + 1, data);
	while (token[ctx.i])
	{
		if (is_escape(token, ctx.i, ctx.quote))
			handle_escape(token, new_token, &ctx);
		else if (is_dollar_quote(token, ctx.i))
			handle_dollar_quote(token, new_token, &ctx);
		else if (is_opening_quote(token[ctx.i], ctx.quote))
			handle_opening_quote(token[ctx.i], &ctx);
		else if (token[ctx.i] == ctx.quote)
			handle_closing_quote(&ctx);
		else
			new_token[ctx.j++] = token[ctx.i++];
	}
	new_token[ctx.j] = '\0';
	gc_free_ptr(token, data);
	return (new_node(new_token, ctx.quoted, data));
}
