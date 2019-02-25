# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcho <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/05 02:16:34 by tcho              #+#    #+#              #
#    Updated: 2018/11/05 02:35:01 by tcho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
BASE = ft_memset \
	   ft_strdup \
	   ft_strfind \
	   ft_strjoin \
	   ft_strlen \
	   ft_strnew \
	   ft_strsub
SRC = $(addsuffix .c, $(BASE))
OBJ = $(addsuffix .o, $(BASE))
HEADER = -I includes
CFLAGS = -Wall -Wextra -Werror -c

all: $(NAME)

$(NAME):
	gcc $(CFLAGS) $(SRC) $(HEADER)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
