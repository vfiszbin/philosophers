# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/19 17:31:34 by vfiszbin          #+#    #+#              #
#    Updated: 2022/06/22 16:37:20 by vfiszbin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS =	philo.c\
		states.c\
		init_philos.c\
		check_end.c\
		utils.c\
		args_checks.c\

OBJS = ${SRCS:.c=.o}

INCLUDES = philo.h

CC = cc

RM = rm -f

FLAGS = -Wall -Wextra -Werror

LIBS = -pthread

.c.o: ${INCLUDES}
	${CC} ${FLAGS} -c $ $< -o ${<:.c=.o}

all: ${NAME}

${NAME} : ${OBJS}
	
	${CC} ${FLAGS} ${OBJS} -o ${NAME} ${LIBS}

clean:
	${RM} *.o

fclean: clean
	${RM} philo

re: fclean all

.PHONY: all clean fclean re