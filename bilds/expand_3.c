/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 03:57:47 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 15:07:54 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void copy_before_index(t_replace_ctx *ctx, char **new_args, int *new_flags)
{
    int i = 0;
    
    i = 0;
    while (i < ctx->index)
    {
        new_args[i] = ctx->cmd->args[i];
        new_flags[i] = ctx->cmd->quoted_flags[i];
        i++;
    }
}

void copy_split(t_replace_ctx *ctx, char **new_args, int *new_flags, int start)
{
    int k = 0;
    while (ctx->split[k])
    {
        new_args[start + k] = ft_strdup(ctx->split[k], ctx->data);
        new_flags[start + k] = 0;
        k++;
    }
}
void copy_after_index(t_replace_ctx *ctx, char **new_args, int *new_flags, int start)
{
    int j;
    int k;
    
    j = start;
    k = ctx->index + 1;
    while ( k < ctx->cmd->argc)
    {
        new_args[j] = ctx->cmd->args[k];
        new_flags[j] = ctx->cmd->quoted_flags[k];
        k++;
        j++;
    }
}

void handle_full_arg_expansion(t_data *data, int *i)
{
    char *tmp = expand_arg(data->com->args[*i], data);
    gc_free_ptr(data->com->args[*i], data);
    data->com->args[*i] = tmp;
    (*i)++;
}

void handle_partial_arg_expansion(t_data *data, int *i)
{
    char *tmp = expand_arg(data->com->args[*i], data);
    gc_free_ptr(data->com->args[*i], data);
    expand_args_one_cmd_1(data->com, tmp, data, i);
}