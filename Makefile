PREFIX ?= /usr/local
SRC = brainfuck.c
CFLAGS = -std=c99 -Wall -pedantic
BIN = bf

.PHONY: clean uninstall

$(BIN):
	$(CC) $(SRC) $(CFLAGS) -o $@

install: $(BIN)
	install $(BIN) $(PREFIX)/bin

clean:
	rm -f $(BIN)

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)
