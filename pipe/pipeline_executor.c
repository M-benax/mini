
#include "../minishell.h"

static t_pid_list *add_pid_to_list(t_pid_list *list_head, pid_t pid, t_data *data)
{
    t_pid_list  *new_node;
    t_pid_list  *current;

    new_node = gc_malloc(sizeof(t_pid_list), data);
    if (!new_node) return (list_head);
    new_node->pid = pid;
    new_node->next = NULL;
    if (!list_head) return (new_node);
    current = list_head;
    while (current->next) current = current->next;
    current->next = new_node;
    return (list_head);
}

static void wait_for_pipeline_pids(t_pid_list *pid_list_head, pid_t last_cmd_pid, t_data *data)
{
    t_pid_list  *current_node;
    int         is_last_in_pipeline;

    current_node = pid_list_head;
    while (current_node)
    {
        is_last_in_pipeline = (current_node->pid == last_cmd_pid);
        handle_parent_wait(current_node->pid, is_last_in_pipeline, data);
        current_node = current_node->next;
    }
}

static pid_t fork_pipeline_segment(t_cmd *cmd, t_data *data, int fd_in,
                                   int fd_out, int pipe_read_to_close_in_child)
{
    pid_t   pid;

    pid = fork();
    if (pid == -1) {
        perror("minishell: fork");
        close(pipe_read_to_close_in_child);
        return (-1);
    }
    if (pid == 0) // Child
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (fd_in != STDIN_FILENO)
            apply_redirection(fd_in, STDIN_FILENO); // Closes fd_in
        if (fd_out != STDOUT_FILENO)
            apply_redirection(fd_out, STDOUT_FILENO); // Closes fd_out
        
        if (pipe_read_to_close_in_child != -1) // If this child writes to a pipe,
                                              // it doesn't need the read end of that *same* pipe.
            close(pipe_read_to_close_in_child);

        execute_command_in_child(cmd, data); // Exits
    }
    return (pid);
}

void execute_pipeline(t_data *data)
{
    t_cmd       *current_cmd;
    int         pipe_fds[2];
    int         input_fd;
    t_pid_list  *pid_list_head;
    pid_t       last_pid_in_pipeline; // Keep track of the actual last PID for $?

    input_fd = STDIN_FILENO;
    current_cmd = data->com;
    pid_list_head = NULL;
    last_pid_in_pipeline = 0;

    while (current_cmd)
    {
        if (current_cmd->next)
        {
            if (pipe(pipe_fds) == -1) {
                perror("minishell: pipe");
                if (input_fd != STDIN_FILENO) close(input_fd);
                gc_free_pid_list(pid_list_head, data);
                data->last_exit_status = EXIT_GENERAL_ERROR; return;
            }
            last_pid_in_pipeline = fork_pipeline_segment(current_cmd, data, input_fd, pipe_fds[1], pipe_fds[0]);
            close(pipe_fds[1]); 
            if (input_fd != STDIN_FILENO) close(input_fd);
            input_fd = pipe_fds[0];
        } else {
            last_pid_in_pipeline = fork_pipeline_segment(current_cmd, data, input_fd, STDOUT_FILENO, -1);
            if (input_fd != STDIN_FILENO) close(input_fd);
        }
        if (last_pid_in_pipeline == -1) { 
            if(current_cmd->next && pipe_fds[0] != -1 && input_fd == pipe_fds[0]) { /* already closed if input_fd was previous pipe */ }
            else if (current_cmd->next && pipe_fds[0] != -1 ) close(pipe_fds[0]); // make sure this read end is closed on error
            gc_free_pid_list(pid_list_head, data); data->last_exit_status = EXIT_GENERAL_ERROR; return;
        }
        pid_list_head = add_pid_to_list(pid_list_head, last_pid_in_pipeline, data);
        if (current_cmd->in_type == T_HEREDOC && current_cmd->in_file) {
             if (access(current_cmd->in_file, F_OK) == 0) unlink(current_cmd->in_file);
        }
        current_cmd = current_cmd->next;
    }
    wait_for_pipeline_pids(pid_list_head, last_pid_in_pipeline, data);
    gc_free_pid_list(pid_list_head, data);
}
