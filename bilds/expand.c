/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:52:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/26 15:27:53 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void replace_arg(t_cmd *cmd, int *i_ptr, char **split, t_data *data)
{
    int split_count = count_args(split);
    replace_arg_at(cmd, *i_ptr, split, data);
    if (split_count > 1)
        *i_ptr += split_count - 1;
    else if (split_count == 0)
        (*i_ptr)--;
}

void expand_args_one_cmd_1(t_cmd *cmd, char *expanded, t_data *data, int *i)
{
    char    **split;
    int k;

    split = ft_split(expanded, ' ', data);
    gc_free_ptr(expanded, data);

    replace_arg(cmd, i, split, data);

    k = 0;
    while (split[k] != NULL) 
    {
        gc_free_ptr(split[k], data);
        k++;
    }
    gc_free_ptr(split, data);
}

static void expand_args_one_cmd(t_data *data)
{
    int i = 0;

    while (i >= 0 && i < data->com->argc && data->com->args[i])
    {
        if (should_skip_expansion(data, i))
        {
            i++;
            continue;
        }
        if (!ft_strchr(data->com->args[i], '$'))
        {
            i++;
            continue;
        }
        if (should_expand_full_arg(data->com->args[i], data, i))
            handle_full_arg_expansion(data, &i);
        else
            handle_partial_arg_expansion(data, &i);
    }
}

int expand_all_commands(t_data *data)
{
    t_cmd *cmd;

    cmd = data->com;
    while (cmd)
    {
        expand_args_one_cmd(data);
        cmd = cmd->next;
    }
    return 0;
}


