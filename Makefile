CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$$HOME/.brew/opt/readline/include
SDIR = sources
ODIR = objects
HDIR = headers
NAME = minishell
MYLIB = mylib/libmylib.a
PIPEXLIB = pipex/pipex.a
RLLIB = -L$$HOME/.brew/opt/readline/lib -lreadline
SRC = ms_readline.c ms_minishell.c ms_lexer.c ms_parser.c ms_utils.c \
ms_is_valid.c ms_initialize.c ms_simpleCmd.c ms_expander.c ms_executor.c \
ms_syntax.c ms_builtins.c ms_str_utils.c ms_simpleCmd2.c ms_builtins2.c \
ms_syntax2.c ms_parser2.c ms_read_simple_cmd.c ms_quote_removal.c \
ms_builtins3.c

$(NAME): $(MYLIB) $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file)) $(PIPEXLIB)
	$(CC) -o $@ $(foreach src,$(SRC:.c=.o),$(ODIR)/$(src)) $(MYLIB) $(RLLIB) $(PIPEXLIB)
$(ODIR)/%.o: $(SDIR)/%.c $(HDIR)/ft_minishell.h
	cd $(ODIR) && $(CC) $(CFLAGS) -c ../$<
$(MYLIB):
	$(MAKE) --directory=mylib
$(PIPEXLIB): pipex/sources/*.c $(HDIR)/*.h
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
