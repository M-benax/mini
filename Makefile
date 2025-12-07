# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/01 21:54:06 by aaboudra          #+#    #+#              #
#    Updated: 2025/08/04 09:01:29 by aaboudra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = main/minishell.c ./Parsing/tokenization.c ./utils/utils.c  \
	./Parsing/flags.c ./Parsing/validation.c memory_management/ft_malloc.c \
	./Parsing/parse.c ./utils/utils_2.c ./segnal/signals.c bilds/bilds.c  \
	 ./utils/ft_split.c Parsing/init_tenv.c bilds/utils_bilds.c bilds/export.c\
	 bilds/expand.c bilds/unset.c bilds/utils_bi2.c main/parcing.c \
	 pipe/pipeline_executor.c pipe/pipeline_executor_utils.c pipe/pipeline_utils.c bilds/builtin_cd.c bilds/builtin_echo.c \
	 bilds/builtin_exit.c bilds/builtin_pwd.c bilds/builtins_core.c \
	 execution/execution_utils.c execution/execution.c execution/execution_utils_03.c execution/execution_utils_04.c execution/execution_helper_utils.c \
	 redirections/redirection_handler.c path/path_handler.c path/path_handler_utils.c \
	 memory_management/memrory_helper.c Parsing/parcer_helper.c \
	 bilds/exapnd_2.c bilds/expand_1.c utils/utils_3.c Parsing/handel_quots.c\
	 bilds/expand_3.c bilds/expand_line.c bilds/expand_helper.c bilds/builtins_helper.c \
	 execution/execution_helper.c execution/execution_utils_helper.c \
	 bilds/builtin_cd_utils.c bilds/builtin_cd_dir.c bilds/builtin_cd_dir_utils.c \
	 Parsing/heredoc_utils.c Parsing/heredoc_parent.c Parsing/handel_q_help.c redirections/reder_help.c \
	 Parsing/env_utils.c Parsing/tokenization_utils.c Parsing/parse_utils.c Parsing/validation_utils.c Parsing/parse_h.c 
	 

CC = cc 
CFLAGS = -Wall -Wextra -Werror
OBJ = $(SRC:.c=.o)
HEADER = minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -lreadline -o $@
	@echo "Compilation of $(NAME) is complete!"

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS)  -c $< -o $@
	@echo "Compiled $< into $@"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all