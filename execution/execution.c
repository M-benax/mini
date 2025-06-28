/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:38:30 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/28 20:07:42 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

void start_execution(t_data *data)
{
    if (!data || !data->com)
        return;
    if (!data->com->args || !data->com->args[0] || data->com->argc == 0)
    {
        data->last_exit_status = 0;
        return;
    }
    if (!data->com->next && is_builtin(data->com->args[0]))
        execute_builtin_parent(data->com, data);
    else if (data->com->next)
        execute_pipeline(data);
    else
        execute_single_external_command(data->com, data);
}

void execute_command_in_child(t_cmd *cmd, t_data *data)
{
    char    *executable_path;
    char    **envp_array;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (handle_child_redirections(cmd) != 0)
        exit(EXIT_GENERAL_ERROR);

    if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
        exit(execute_builtin_child(cmd, data));

    if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
        exit(0);

    executable_path = find_executable_path(cmd->args[0], data->tenv, data);
    
    if (!executable_path)
    {
        if (ft_strchr(cmd->args[0], '/'))
        {
            if (access(cmd->args[0], F_OK) == 0)
            {
                display_error_message(cmd->args[0], NULL, "Permission denied");
                exit(EXIT_CMD_CANNOT_EXECUTE);
            }
            else
            {
                display_error_message(cmd->args[0], NULL, "No such file or directory");
                exit(EXIT_CMD_NOT_FOUND);
            }
        }
        else
        {
            char *path_env = get_env_value(data->tenv, "PATH", data);
            if (!path_env || path_env[0] == '\0')
            {
                // Check if the command exists in current directory
                if (access(cmd->args[0], F_OK) == 0)
                {
                    display_error_message(cmd->args[0], NULL, "Permission denied");
                }
                else
                {
                    display_error_message(cmd->args[0], NULL, "No such file or directory");
                }
                if (path_env) gc_free_ptr(path_env, data);
            }
            else
            {
                display_error_message(cmd->args[0], NULL, "command not found");
                gc_free_ptr(path_env, data);
            }
            exit(EXIT_CMD_NOT_FOUND);
        }
    }
    envp_array = convert_env_list_to_array(data->tenv, data);
    if (!envp_array) { gc_free_ptr(executable_path, data); exit(EXIT_GENERAL_ERROR); }
    
    execve(executable_path, cmd->args, envp_array);

    display_error_message(cmd->args[0], NULL, strerror(errno));
    gc_free_ptr(executable_path, data);
    gc_free_array(envp_array, data);
    if (errno == EACCES) exit(EXIT_CMD_CANNOT_EXECUTE);
    if (errno == ENOENT) exit(EXIT_CMD_NOT_FOUND);
    exit(EXIT_GENERAL_ERROR);
}

void execute_single_external_command(t_cmd *cmd, t_data *data)
{
    pid_t   pid;

    if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0') {
        data->last_exit_status = 0;
        return;
    }
    pid = fork();
    if (pid == -1) {
        perror("minishell: fork");
        data->last_exit_status = EXIT_GENERAL_ERROR;
        return;
    }
    if (pid == 0)
        execute_command_in_child(cmd, data);
    else
    {
        handle_parent_wait(pid, 1, data);
        if (cmd->in_type == T_HEREDOC && cmd->in_file)
        {
            if (access(cmd->in_file, F_OK) == 0) unlink(cmd->in_file);
        }
    }
}
