CC=gcc
CFLAGS=-O0 -ggdb3 -std=c11 -Wall -Wno-discarded-qualifiers -pedantic -lpthread
SRC=$(wildcard src/*.c)

all: release

clean:
	rm -rf bin/
	rm -f assets/feedback.csv

create_teachers:
	mkdir -p assets/
	echo Ranjana Jadhav > assets/teachers.txt
	echo Pankaj Kunekar >> assets/teachers.txt
	echo Preeti Bailke >> assets/teachers.txt
	echo Kuldeep Vaydande >> assets/teachers.txt
	echo Deepali Deshpande >> assets/teachers.txt

release:
	mkdir -p bin/
	${CC} ${CFLAGS} -s -DNDEBUG ${SRC} -o bin/CN_Server
	chmod +x bin/CN_Server
