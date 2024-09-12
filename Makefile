NAME		:=	philo

CFLAGS		:=	-Wextra -Wall -Werror -g
LDFLAGS		:=	-pthread #-fsanitize=thread

HEADERS		:=	-Iinclude

SRCS_DIR	:=	./src/
SRCS 		:=	philo.c		\
				init.c		\
				routine.c	\
				monitor.c	\
				utils.c

SRCS		:=	$(addprefix $(SRCS_DIR), $(SRCS))

OBJS		:=	${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	@echo "Compiling $< to $@..."
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(LIBFT) $(LIBFTPRINTF) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@echo "Linking object files to create $(NAME)..."
	@echo "\n'$(NAME)' binary successfully created."

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJS)

fclean: clean
	@echo "\nPerforming full clean..."
	rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
