/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:02:24 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/02 16:43:02 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// No custom signal handler needed

int	handel_heredoc(char *limiter, t_cmd *cmd, t_data *data)
{
	int fd;
	char *file_name;
	pid_t pid;
	int status;
	void (*old_sigint)(int);
	void (*old_sigquit)(int);
	
	fd = open_file(&file_name, data);
	
	// Save current signal handlers
	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	
	pid = fork();
	
	if (pid == -1)
	{
		perror("fork");
		close(fd);
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		return (1);
	}
	
	if (pid == 0) // Child process
	{
		char *line;
		char *expanded_line;
		
		// Use default signal handling in child
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				write(STDERR_FILENO, "bash: warning: here-document delimited by end-of-file (wanted `", 62);
				write(STDERR_FILENO, limiter, ft_strlen(limiter));
				write(STDERR_FILENO, "')\n", 3);
				break;
			}
			if (ft_strcmp(line, limiter) == 0)
			{
				free(line);
				break;
			}
			expanded_line = expand_line(line, data);
			write(fd, expanded_line, ft_strlen(expanded_line));
			write(fd, "\n", 1);
			gc_free_ptr(expanded_line, data);
			free(line);
		}
		close(fd);
		exit(0);
	}
	
	// Parent process
	close(fd);
	waitpid(pid, &status, 0);
	
	// Restore signal handlers
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(file_name);
		return (1);
	}
	
	cmd->in_file = file_name;
	return (0);
}
void	copy_args(char **src, char **dst, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}

void	copy_flags(int *src, int *dst, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}
