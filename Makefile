# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/18 18:33:48 by acastrov          #+#    #+#              #
#    Updated: 2025/01/03 23:07:16 by acastrov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS = pipex.c \
	pipex_args.c \
	pipex_free.c \
	pipex_check.c \
	pipex_open.c \
	pipex_libft_other_1.c \
	pipex_libft_other_2.c \
	pipex_libft_split.c \

OBJS = $(SRCS:.c=.o)

CFLAGS = -Werror -Wextra -Wall

CC = cc

all: $(NAME)

$(NAME): $(SRCS)
	cc $(SRCS) -o $(NAME)
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
