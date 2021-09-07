CC = gcc
CFLAGS = -Wall -Wextra -Werror
SDIR = sources
ODIR = objects
NAME = minishell
MYLIB = mylib/libmylib.a
PIPEXLIB = pipex/pipex.a
RLLIB = -lreadline
SRC = readline.c minishell.c stack_operations.c convertStrToTokens.c \
isValidCmdLine.c parseTokens.c

$(NAME): $(MYLIB) $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file)) $(PIPEXLIB)
	$(CC) -o $@ $(foreach src,$(SRC:.c=.o),$(ODIR)/$(src)) $(MYLIB) $(RLLIB) $(PIPEXLIB)
$(ODIR)/%.o: $(SDIR)/%.c
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