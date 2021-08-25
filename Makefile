CC = GCC
CFLAGS =
SDIR = sources
ODIR = objects
NAME = minishell
MYLIB = mylib/libmylib.a

$(NAME): $(SRC:.c=.o) $(MYLIB)
	$(CC) -o $@ $(foreach src,$(SRC:.c=.o),$(ODIR)/$(src)) $(MYLIB)
%.o: $(SDIR)/%.c
	cd $(ODIR) && $(CC) $(CFLAGS) -c ../$<
$(MYLIB):
	$(MAKE) --directory=mylib

.PHONY: all clean fclean re bonus
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