CC=gcc
CFLAGS=-O0 -ggdb3 -std=c11 -Wall -Wno-discarded-qualifiers -pedantic -lpthread
SRC=$(wildcard src/*.c)

all: release

clean:
	rm -rf bin/

release:
	mkdir -p bin/
	${CC} ${CFLAGS} -s -DNDEBUG ${SRC} -o bin/CN_Client
	chmod +x bin/CN_Client
