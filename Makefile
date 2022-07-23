CC=gcc
CCFLAGS=-Wall -Wextra
LD=ld
lib_sources = $(wildcard src/lib/*.c)

default: build

build:
	mkdir -p dist/

	$(CC) src/sse.c $(lib_sources) -o dist/sse $(CCFLAGS)
clean:
	rm -rf ./dist

run: clean build
	dist/sse