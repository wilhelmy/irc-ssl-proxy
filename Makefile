CC=cc
LD=$(CC)
SRC=ircproxy.c
OBJ=$(SRC:.c=.o)

ircproxy: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $>
.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $>

clean:
	rm -f ircproxy $(OBJ)
.PHONY: clean
