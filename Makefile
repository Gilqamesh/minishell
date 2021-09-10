CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SDIR = sources
ODIR = objects
HDIR = headers
NAME = minishell
MYLIB = mylib/libmylib.a
PIPEXLIB = pipex/pipex.a
RLLIB = -lreadline
SRC = readline.c minishell.c stack_operations.c lexer.c \
parser.c utils.c isValid.c error.c ms_initialize.c ms_shell_node.c \
debug_funcs.c expander.c

$(NAME): $(MYLIB) $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file)) $(PIPEXLIB)
	$(CC) -o $@ $(foreach src,$(SRC:.c=.o),$(ODIR)/$(src)) $(MYLIB) $(RLLIB) $(PIPEXLIB)
$(ODIR)/%.o: $(SDIR)/%.c $(HDIR)/ft_minishell.h
	cd $(ODIR) && $(CC) $(CFLAGS) -c ../$<
$(MYLIB):
	$(MAKE) --directory=mylib
$(PIPEXLIB):
	$(MAKE) --directory=pipex

.PHONY: all clean fclean re bonus fcleanall
all:
	make $(NAME)
clean:
	rm -f objects/*.o
fclean:
	make clean
	rm -f $(NAME)
re:
	make clean
	make all
bonus:
	make $(NAME)
fcleanall:
	make fclean
	$(MAKE) fcleanall --directory=mylib
	$(MAKE) fcleanall --directory=pipex