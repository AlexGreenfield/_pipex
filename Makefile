# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/18 18:33:48 by acastrov          #+#    #+#              #
#    Updated: 2025/01/03 17:40:53 by acastrov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex.a

LIBFT = libft.a

LIBFTDIR = ./libft

SRCS = pipex.c \
	pipex_args.c \
	pipex_free.c \
	pipex_check.c \
	pipex_open.c \

OBJS = $(SRCS:.c=.o)

CFLAGS = -Werror -Wextra -Wall

CC = cc

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFTDIR)/$(LIBFT)
	@cp $(LIBFTDIR)/$(LIBFT) $(NAME)
	@ar -rcs $(NAME) $(OBJS)

$(LIBFTDIR)/$(LIBFT):
	@make -C $(LIBFTDIR) CFLAGS="$(CFLAGS)"

clean:
	@rm -f $(OBJS)
	@cd $(LIBFTDIR) && make clean

fclean: clean
	@rm -f $(NAME)
	@cd $(LIBFTDIR) && make fclean

re: fclean all

.PHONY: all clean fclean re
