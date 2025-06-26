/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:35:16 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/24 19:27:28 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token_type	chek_type(t_comand *tocken)
{
	if (!ft_strcmp(tocken->word, "<"))
		return (T_REDIR_IN);
	if (!ft_strcmp(tocken->word, "<<"))
		return (T_HEREDOC);
	if (!ft_strcmp(tocken->word, ">"))
		return(T_REDIR_OUT);
	if (!ft_strcmp(tocken->word, ">>"))
		return (T_APPEND);
	if (!ft_strcmp(tocken->word, "|"))
		return(T_PIPE);
	return (T_WORD);
}

void	put_type(t_comand *token)
{
	t_token_type type;

	type = chek_type(token);
	token->type = type;
}

