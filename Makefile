SRCS = src/main.c src/UI/ui.c
INCLUDE_DIR = include
BIN = writing-app

CC = /usr/bin/cc
CFLAGS_DEBUG = -O0 -Wall -Wextra -Wno-unused-parameter -std=gnu17 -pedantic -fms-extensions -ggdb
CFLAGS_RELEASE = -O3 -Wall -Wextra -std=gnu17 -pedantic -fms-extensions

LIBS = $(shell pkg-config glfw3 --static --libs) -lGL -lfreetype -I/usr/include/freetype2


build_debug: $(SRCS)
	$(CC) $(CFLAGS_DEBUG) $^ -o $(BIN) $(LIBS) -I $(INCLUDE_DIR)

build_release: $(SRCS)
	$(CC) $(CFLAGS_RELEASE) $^ -o $(BIN) $(LIBS) -I $(INCLUDE_DIR)

run: build
	./$(BIN)

clean:
	rm -rf $(BIN)


.PHONY: build clean run