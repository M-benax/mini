/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:52:12 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/27 13:13:30 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main(int ac, char **av, char **env)
{
	t_data *data;
	
	(void)ac;
	(void)av;
	data = malloc(sizeof(t_data));
	if (!data)
   		exit(EXIT_FAILURE);
	data->last_exit_status = 0;
	data->g_gc_list = NULL;
	data->tenv = gc_malloc(sizeof(t_env), data);
	data->tenv = init_env(env, data);
	parcing(data);
	gc_free_all(data);
	rl_clear_history();
}













































// typedef struct s_cmd
// {
//     char **args;       // arguments
//     int    in_type;    // < أو <<
//     char  *in_file;    // الملف لي غادي يقرا منو
//     int    out_type;   // > أو >>
//     char  *out_file;   // الملف لي غادي يكتب فيه
//     struct s_cmd *next; // باش نربطها مع command أخرى (بسبب pipes)
//     s_data_env env;
// } t_cmd;
