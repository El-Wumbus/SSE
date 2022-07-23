CCX=g++
CCXFLAGS=-Wall -Wextra
LD=ld
lib_sources = $(wildcard src/lib/*.c++)

default: build

build:
	mkdir -p dist/
	mkdir -p dist/lib

	for file in $(lib_sources); do \
		$(CCX) $$file -o dist/lib/$$(scripts/replace_file_extention.py $$(basename $$file) .o) $(CXXFLAGS); \
	done

	$(CCX) src/sse.c++ -o dist/sse.o $(CXXFLAGS)
	# $(LD) -o dist/sse dist/sse.o $$(ls dist/lib/*.o)

clean:
	rm -rf ./dist

run: clean build
	dist/sse.o