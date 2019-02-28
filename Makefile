# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcho <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/17 23:47:20 by tcho              #+#    #+#              #
#    Updated: 2019/02/24 02:56:58 by tcho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ftp
CLIENT = client/client2
SERVER = server/server2
CLIENT_SRC = $(addsuffix .c, $(CLIENT))
SERVER_SRC = $(addsuffix .c, $(SERVER))
CLIENT_EXE = c
SERVER_EXE = s
INCLUDE = -I. -I./get_next_line -I./libft
LIB = -L./libft -lft
GNL = ./get_next_line/get_next_line.c
CC = gcc -Wall -Wextra -Werror
SANITIZE = -fsanitize=address

all: $(NAME)

$(NAME): $(CLIENT_SRC) $(SERVER_SRC) $(LIBFT)
	$(CC) $(CLIENT_SRC) $(GNL) $(INCLUDE) $(LIB) -o $(CLIENT_EXE) $(SANITIZE)
	$(CC) $(SERVER_SRC) $(GNL) $(INCLUDE) $(LIB) -o $(SERVER_EXE) $(SANITIZE)

$(LIBFT):
	make -C ./libft

clean:
	make clean -C ./libft

fclean:
	/bin/rm -f $(CLIENT_EXE)
	/bin/rm -f $(SERVER_EXE)
	make fclean -C ./libft

re: fclean all
