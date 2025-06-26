/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:38:11 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 19:38:12 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

void handle_parent_wait(pid_t child_pid, int is_pipeline_last_cmd, t_data *data)
{
    int status;
    int child_exit_status;

    waitpid(child_pid, &status, 0);
    child_exit_status = 0;
    if (WIFEXITED(status))
        child_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        child_exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(STDERR_FILENO, "Quit: 3\n", 8);
        else if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
    }
    if (is_pipeline_last_cmd)
        data->last_exit_status = child_exit_status;
}

char **convert_env_list_to_array(t_env *env_list, t_data *data)
{
    int     count;
    t_env   *tmp;
    char    **env_array;
    int     i;
    char    *var_eq_val;
    size_t  len_var;
    size_t  len_val;

    tmp = env_list;
    count = 0;
    while (tmp) {
        if (tmp->val) count++;
        tmp = tmp->next;
    }
    env_array = gc_malloc(sizeof(char *) * (count + 1), data);
    if (!env_array) return (NULL);
    tmp = env_list;
    i = 0;
    while (tmp) {
        if (tmp->val) {
            len_var = ft_strlen(tmp->var);
            len_val = ft_strlen(tmp->val);
            var_eq_val = gc_malloc((len_var + 1 + len_val + 1), data);
            if (!var_eq_val) { /* TODO: Handle malloc error */ gc_free_array(env_array, data); return NULL;}
            ft_memcpy(var_eq_val, tmp->var, len_var);
            var_eq_val[len_var] = '=';
            ft_memcpy(var_eq_val + len_var + 1, tmp->val, len_val);
            var_eq_val[len_var + 1 + len_val] = '\0';
            env_array[i++] = var_eq_val;
        }
        tmp = tmp->next;
    }
    env_array[i] = NULL;
    return (env_array);
}

void display_error_message(char *command, char *arg, char *message)
{
    write(STDERR_FILENO, "minishell: ", 11);
    if (command)
    {
        write(STDERR_FILENO, command, ft_strlen(command));
        write(STDERR_FILENO, ": ", 2);
    }
    if (arg)
    {
        write(STDERR_FILENO, arg, ft_strlen(arg));
        write(STDERR_FILENO, ": ", 2);
    }
    write(STDERR_FILENO, message, ft_strlen(message));
    write(STDERR_FILENO, "\n", 1);
}
