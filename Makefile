
PREFIX ?= /usr/local
SRC = brainfuck.c
CFLAGS = -std=c99
BIN = bf

$(BIN):
	$(CC) $(SRC) $(CFLAGS) -o $@

install:
	install $(BIN) $(PREFIX)/bin

clean:
	rm -f $(BIN)

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)
