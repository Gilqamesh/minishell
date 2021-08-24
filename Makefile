CC = GCC
CFLAGS =
SDIR = sources
ODIR = objects
NAME = minishell
MYLIB = mylib/libmylib.a

.PHONY: all clean fclean re bonus
all:
	make $(NAME)
