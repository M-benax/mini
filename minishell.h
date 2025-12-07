/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:54:20 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/08 16:58:00 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <limits.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_HEREDOCS 15
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CMD_CANNOT_EXECUTE 126
# define EXIT_SYNTAX_ERROR 2
# define EXIT_GENERAL_ERROR 1

extern volatile sig_atomic_t	g_sigint_received;

typedef struct s_pid_list
{
	pid_t				pid;
	struct s_pid_list	*next;
}	t_pid_list;

typedef struct s_env
{
	char			*var;
	char			*val;
	int				flage;
	struct s_env	*next;
}	t_env;

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

typedef struct s_cmd
{
	char			**args;
	int				*quoted_flags;
	int				argc;
	int				in_type;
	char			*in_file;
	int				out_type;
	char			*out_file;
	int				has_redirect_error;
	int				q_limeter;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	t_env			*tenv;
	t_cmd			*com;
	int				last_exit_status;
	t_gc_node		*g_gc_list;
}	t_data;

typedef struct s_comand
{
	t_token_type		type;
	char				*word;
	int					quoted;
	struct s_comand		*next;
}	t_comand;

typedef struct s_quote_ctx
{
	int			i;
	int			j;
	char		quote;
	int			quoted;
}	t_quote_ctx;

typedef struct s_replace_ctx
{
	t_cmd		*cmd;
	int			index;
	char		**split;
	t_data		*data;
}	t_replace_ctx;

typedef struct s_tok_parts
{
	char		*lhs;
	char		*rhs;
}	t_tok_parts;

typedef struct s_expand
{
	const char	*line;
	char		*result;
	size_t		i;
	size_t		j;
	t_data		*data;
}	t_expand;

typedef struct s_cd_data
{
	char	*pwd_value;
	char	*old_pwd;
	char	*target_path;
	t_env	**env_list;
	t_data	*data;
}	t_cd_data;

size_t			get_var_name(const char *line, size_t *i, char *var_name);
int				parse_heredoc_if_needed(t_comand **tokens, t_cmd *node,
					t_data *data);
int				open_output_file(char *filename, int type);
int				open_input_file(char *filename);
int				handle_parent_redirections(t_cmd *cmd);
t_comand		*tokens(char *input, t_data *data);
void			parcing(t_data *data);
void			parse_command(t_comand *tokens, t_data *data);
void			biltings(t_data *comand);
int				error_pipe(char *line);
int				is_space(char c);
int				is_s_char(char c);
size_t			ft_strlen(const char *s);
char			*ft_strndup(const char *s1, size_t size, t_data *data);
void			*ft_memcpy(void *dst, const void *src, size_t n);
t_comand		*new_node(char *token, int q, t_data *data);
char			*ft_strdup(const char *s1, t_data *data);
void			put_type(t_comand *tokens);
int				ft_strcmp(char *s1, char *s2);
int				check_syntax(t_comand *tokens);
void			parse_command(t_comand *tokens, t_data *data);
int				handel_heredoc(char *limiter, t_cmd *cmd, t_data *data);
int				open_file(char **filename, t_data *data);
char			*generate_tmp_filename(t_data *data);
char			*ft_strjoin(char const *s1, char const *s2, t_data *data);
char			*ft_itoa(int n, t_data *data);
void			*gc_malloc(size_t size, t_data *data);
void			gc_free_all(t_data *data);
t_comand		*handle_quotes(char *token, t_data *data);
void			_sigint_handler(int sig);
void			signal_init(void);
int				chech_syntax(t_comand *check, char *line, t_data *data);
char			**ft_split(char const *s, char c, t_data *data);
void			biltings(t_data *comand);
t_env			*init_env(char **envp, t_data *data);
void			print_env(t_env *env);
int				count_nodes(t_env *env);
int				handel_export(t_data *command);
char			*get_var_plus_equal(char *arg, t_data *data);
char			*get_val_plus_equal(char *arg, t_data *data);
t_env			*new_env_node(char *var, char *val, t_data *data);
int				expand_all_commands(t_data *data);
char			*ft_strcpy(char *s1, char *s2);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strchr(const char *s, int c);
int				unset(char **args, t_env **env);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
char			*get_env_value(t_env *env, char *key, t_data *data);
void			gc_free_ptr(void *ptr, t_data *data);
int				unclosed_quote(const char *str);
void			copy_args(char **src, char **dst, int count);
void			copy_flags(int *src, int *dst, int count);
int				handle_dollar(const char *s, char **res, int *i, t_data *d);
void			replace_arg_at(t_cmd *cmd, int index, char **split,
					t_data *data);
int				count_args(char **args);
char			*expand_var(const char *str, t_data *data);
char			*append_char_to_result(char *result, char c, t_data *data);
int				should_expand_full_arg(char *arg, t_data *data, int i);
char			*expand_arg(char *arg, t_data *data);
int				should_skip_expansion(t_data *data, int i);
void			copy_before_index(t_replace_ctx *ctx, char **new_args,
					int *new_flags);
void			copy_split(t_replace_ctx *ctx, char **new_args,
					int *new_flags, int start);
void			copy_after_index(t_replace_ctx *ctx, char **new_args,
					int *new_flags, int start);
void			handle_full_arg_expansion(t_data *data, int *i);
void			handle_partial_arg_expansion(t_data *data, int *i);
void			expand_args_one_cmd_1(t_cmd *cmd, char *expanded,
					t_data *data, int *i);
char			*expand_line(const char *line, t_cmd *cmd, t_data *data);
size_t			calc_var_len(const char *line, size_t *i, t_data *data);
int				is_var_start(char c);
int				is_var_char(char c);
int				kontine_puse(char *argg);
void			handle_escape(char *token, char *new_token, t_quote_ctx *ctx);
int				is_escape(char *token, int i, char quote);
int				is_dollar_quote(char *token, int i);
int				is_opening_quote(char c, char quote);
void			handle_closing_quote(t_quote_ctx *ctx);
void			handle_heredoc_child(int fd, t_cmd *cmd, char *limiter,
					t_data *data);
void			restore_signals_and_close(int fd, void (*old_sigint)(int),
					void (*old_sigquit)(int));
int				setup_heredoc_fork(int *fd, char **file_name, t_data *data,
					void (**old_handlers)(int));
int				handle_heredoc_parent(pid_t pid, char *file_name, t_cmd *cmd,
					void (**old_handlers)(int));
void			process_heredoc_line(int fd, t_cmd *cmd, char *line,
					t_data *data);
void			print_data_cmd(t_data *data);
void			start_execution(t_data *data);
int				is_builtin(char *cmd_name);
void			execute_builtin_parent(t_cmd *cmd, t_data *data);
void			execute_single_external_command(t_cmd *cmd, t_data *data);
int				execute_builtin_child(t_cmd *cmd, t_data *data);
void			execute_command_in_child(t_cmd *cmd, t_data *data);
void			execute_pipeline(t_data *data);
int				builtin_exit(char **args, t_data *data);
int				builtin_cd(char **args, t_env **env_list, t_data *data);
int				change_directory(char *target_path, t_env **env_list,
					t_data *data);
void			set_env_var(t_env **env_list, char *var_name,
					char *var_value, t_data *data);
char			*get_target_path(char **args, t_env *env_list, t_data *data);
char			*construct_new_path(char *old_path, char *target,
					t_data *data);
void			display_getcwd_error(char *arg, char *message);
int				handle_getcwd_failure(char *pwd_value, char *target_path,
					char *old_pwd, t_data *data);
char			*extract_parent_dir(char *pwd_value, t_data *data);
int				handle_parent_dir(char *pwd_value, t_env **env_list,
					char *old_pwd_buffer, t_data *data);
int				builtin_echo(char **args);
int				builtin_pwd(t_env *env_list, t_data *data);
void			handle_parent_wait(pid_t child_pid, int is_pipeline_last_cmd,
					t_data *data);
char			**convert_env_list_to_array(t_env *env_list, t_data *data);
void			display_error_message(char *command, char *arg, char *message);
void			gc_free_pid_list(t_pid_list *list, t_data *data);
t_pid_list		*add_pid_to_list(t_pid_list *list_head, pid_t pid,
					t_data *data);
void			wait_for_pipeline_pids(t_pid_list *pid_list_head,
					pid_t last_cmd_pid, t_data *data);
pid_t			fork_pipeline_segment(t_cmd *cmd, t_data *data,
					int fd_in, int fda[2]);
void			handle_pipe_error(int input_fd, t_pid_list *pid_list_head,
					t_data *data);
int				handle_pipe_cmd(t_cmd *cmd, t_data *data, int *input_fd);
int				handle_last_cmd(t_cmd *cmd, t_data *data, int input_fd);
int				process_cmd(t_cmd *cmd, t_data *data, int *input_fd);
void			pipeline_cleanup(t_pid_list *pid_list_head, pid_t last_pid,
					t_data *data);
t_pid_list		*process_pipeline_cmds(t_data *data, int *input_fd,
					pid_t *last_pid);
void			cleanup_heredoc(t_cmd *cmd);
void			apply_redirection(int old_fd, int new_fd);
int				ft_atoi(const char *str);
void			print_export(t_env *env, t_data *data);
int				is_str_numeric(const char *str);
int				count_args(char **args);
void			gc_free_array(char **array, t_data *data);
int				handle_child_redirections(t_cmd *cmd);
int				handle_parent_redirections(t_cmd *cmd);
char			*find_executable_path(char *cmd_name, t_env *env_list,
					t_data *data);
char			*join_path_cmd(const char *path, const char *cmd,
					t_data *data);
char			*try_current_dir(char *cmd_name, t_data *data);
char			*check_paths_for_cmd(char **paths, char *cmd_name,
					t_data *data);
int				exec_builtin_cmd(t_cmd *cmd, t_data *data);
void			handle_path_error(t_cmd *cmd, t_data *data);
void			handle_no_executable(t_cmd *cmd, t_data *data);
int				is_minishell_command(char *cmd_name);
void			execute_with_path(t_cmd *cmd, t_data *data,
					char *executable_path);
int				exec_builtin_cmd(t_cmd *cmd, t_data *data);
void			handle_builtin_redirections(t_cmd *cmd, int *stdout_backup,
					int *stdin_backup);
void			restore_builtin_redirections(int stdout_backup,
					int stdin_backup);
void			handle_redirection(t_cmd *cmd, t_comand *token, t_data *data,
					int redir_type);
t_cmd			*init_node(t_data *data);
void			add_arg(t_cmd *cmd, char *word, int q, t_data *data);
void			put_command_part(t_comand *token, t_cmd *cmd, t_data *data);
char			*ft_substr(const char *s, unsigned int start, size_t len,
					t_data *data);
char			*handel_tok(char *token, t_data *data);
t_env			*create_env_node(char **envp, int i, t_data *data);
t_env			*parse_envp(char **envp, t_data *data);
void			update_pwd_env_var(t_cd_data *cd, char *new_pwd_buffer);
int				check_getcwd_status(t_cd_data *cd, char *new_pwd_buffer);
int				try_chdir_and_handle_failure(t_cd_data *cd);
int				handle_getcwd_failure_internal(t_cd_data *cd);
void			replace_arg(t_cmd *cmd, int *i_ptr, char **split, t_data *data);
void			signal_init1(void);
void			handle_sigint(t_data *data);
void			handle_eof(t_data *data);

#endif
