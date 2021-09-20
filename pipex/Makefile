CC = gcc
CFLAGS = -Wall -Wextra -Werror
SDIR = sources
ODIR = objects
SRC = pipex_pipex.c pipex_fork.c pipex_error.c pipex_initialize.c \
pipex_here_doc.c pipex_myfuncs.c
NAME = pipex.a
MYLIBDIR = ../mylib
MYLIB = $(MYLIBDIR)/libmylib.a
HEADERLIB = ../headers

$(NAME): $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file)) $(MYLIB)
	ar -rs $(NAME) $^
$(ODIR)/%.o: $(SDIR)/%.c $(HEADERLIB)/*.h
	cd $(ODIR) && $(CC) $(CFLAGS) -c ../$<
$(MYLIB):
	$(MAKE) --directory=$(MYLIBDIR)

.PHONY: clean re all fclean bonus fcleanall
all:
	make $(NAME)
clean:
	rm -f $(ODIR)/*.o
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
	$(MAKE) fcleanall --directory=$(MYLIBDIR)
