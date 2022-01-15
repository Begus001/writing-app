SRCS = src/main.c src/UI/ui.c
INCLUDE_DIR = include
BIN = writing-app

CC = /usr/bin/cc
CFLAGS = -O0 -Wall -Wextra -std=gnu17 -pedantic -fms-extensions

LIBS = $(shell pkg-config glfw3 --static --libs) -lGL

build: $(SRCS).o
	$(CC) $< -o $(BIN) $(LIBS) -I $(INCLUDE_DIR)

$(SRCS).o: $(SRCS)
	$(CC) -c $(CFLAGS) $< -o $@ -I $(INCLUDE_DIR)

run: build
	./$(BIN)

clean:
	rm -rf $(BIN) $(SRCS).o

.PHONY: build clean run