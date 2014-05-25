#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sconso <sconso@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/01/12 18:13:08 by sconso            #+#    #+#              #
#    Updated: 2014/05/25 07:32:27 by Myrkskog         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = client
S_NAME = server

CC = gcc
CFLAGS = -Wall -Werror -Wextra
DFLAGS = -pedantic -g -ggdb
INC = -I includes/ -I libft/includes/
LFLAGS = -L libft/ -lft

C_FILES = src/client/client.c
S_FILES = 	src/server/server.c \
			src/server/ft_errors.c \
			src/server/get_message.c

SRCDIR = src/client src/server
C_SRCDIR = src/client
S_SRCDIR = src/server

C_OBJ = $(patsubst src/%,obj/%,$(C_FILES:.c=.o))
S_OBJ = $(patsubst src/%,obj/%,$(S_FILES:.c=.o))
OBJDIR = $(addsuffix .dir,$(C_OBJ)) $(addsuffix .dir,$(S_OBJ))

all : lib $(OBJDIR) $(NAME) $(S_NAME)

$(NAME) : $(C_OBJ)
		@$(CC) $(CFLAGS) $(INC) -o $@ $(C_OBJ) $(LFLAGS)
		@echo "\033[4;32m$@\033[0m created !"

$(S_NAME) : $(S_OBJ)
		@$(CC) $(CFLAGS) $(INC) -o $@ $(S_OBJ) $(LFLAGS)
		@echo "\033[4;32m$@\033[0m created !"

obj/%.dir:
		@mkdir -p $(dir $@)

obj/%.o: src/%.c
		@$(CC) -o $@ -c $< $(CFLAGS) $(INC)

run : re
		@/usr/bin/clear
		@./$(NAME)

clean : libclean
		@/bin/rm -rf obj a.out.dSYM
		@/bin/rm -f a.out
		@echo "\033[1;30mobj\033[0m removed !"

fclean : clean libfclean
		@/bin/rm -f $(NAME)
		@/bin/rm -f $(S_NAME)
		@echo "\033[1;30m$(NAME)\033[0m removed !"
		@echo "\033[1;30m$(S_NAME)\033[0m removed !"

lib :
		@make -C libft all

libclean :
		@make -C libft clean

libfclean :
		@make -C libft fclean

libre :
		@make -C libft re
		@make re

re : fclean all

debug : CFLAGS += $(DFLAGS)
debug : re

.PHONY : all clean fclean re
