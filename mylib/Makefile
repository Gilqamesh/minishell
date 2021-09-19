include Makefile.variable
LIBFT = $(LIBFTDIR)/$(LIBFTNAME)
LIBPRINTF = $(PRINTFDIR)/$(LIBPRINTFNAME)
LIBGNL = $(GNLDIR)/$(LIBGNLNAME)

$(MYLIBNAME): $(LIBFT) $(LIBPRINTF) $(LIBGNL)
$(LIBFT): $(LIBFTDIR)/*.c $(LIBFTDIR)/*.h
	$(MAKE) --directory=$(LIBFTDIR)
	cp $(LIBFT) .
	ar -x $(LIBFTNAME)
	ar -rs $(MYLIBNAME) *.o
	rm -f $(LIBFTNAME)
	make clean
$(LIBPRINTF): $(PRINTFDIR)/*.c $(PRINTFDIR)/*.h
	$(MAKE) --directory=$(PRINTFDIR)
	cp $(LIBPRINTF) .
	ar -x $(LIBPRINTFNAME)
	ar -rs $(MYLIBNAME) *.o
	rm -f $(LIBPRINTFNAME)
	make clean
$(LIBGNL): $(GNLDIR)/*.c $(GNLDIR)/*.h
	$(MAKE) --directory=$(GNLDIR)
	cp $(LIBGNL) .
	ar -x $(LIBGNLNAME)
	ar -rs $(MYLIBNAME) *.o
	rm -f $(LIBGNLNAME)
	make clean
.PHONY: all fclean fcleanall
all: $(MYLIBNAME)
clean:
	rm -rf *.o __.SYMDEF __.SYMDEF\ SORTED
fclean:
	make clean
	rm -rf $(MYLIBNAME)
	rm -f $(LIBFT) $(LIBPRINTF) $(LIBGNL)
fcleanall: fclean
	$(MAKE) fcleanall --directory=$(LIBFTDIR)
	$(MAKE) fcleanall --directory=$(GNLDIR)
	$(MAKE) fcleanall --directory=$(PRINTFDIR)
