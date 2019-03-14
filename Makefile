# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcho <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/17 23:47:20 by tcho              #+#    #+#              #
#    Updated: 2019/03/14 15:27:04 by tcho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ftp
CLIENT = client
SERVER = server
COMMON = common
CLIENT_SRC = $(addsuffix .c, $(CLIENT))
SERVER_SRC = $(addsuffix .c, $(SERVER))
COMMON_SRC = $(addsuffix .c, $(COMMON))
CLIENT_EXE = client
SERVER_EXE = server
INCLUDE = -I. -I ./libft
LIB = -L./libft -lft
CC = gcc -Wall -Wextra -Werror
SANITIZE = -fsanitize=address

all: $(NAME)

$(NAME): $(COMMON_SRC) $(CLIENT_SRC) $(SERVER_SRC)
	make -C ./libft
	$(CC) $(CLIENT_SRC) $(COMMON_SRC) $(INCLUDE) $(LIB) -o $(CLIENT_EXE)
	$(CC) $(SERVER_SRC) $(COMMON_SRC) $(INCLUDE) $(LIB) -o $(SERVER_EXE)

clean:
	make clean -C ./libft

fclean:
	/bin/rm -f $(CLIENT_EXE)
	/bin/rm -f $(SERVER_EXE)
	make fclean -C ./libft

re: fclean all

.PHONY: all $(NAME)
