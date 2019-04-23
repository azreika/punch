CC=g++
CPPFLAGS=-g -Wall -Werror -std=c++17
TARGET=punch

.PHONY: all clean

# TODO: clean up this makefile

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

Parser.o: Parser.cpp Parser.h Token.h AstNode.h AstProgram.h AstStatement.h
	$(CC) -c $(CPPFLAGS) $< -o $@

Scanner.o: Scanner.cpp Scanner.h Token.h
	$(CC) -c $(CPPFLAGS) $< -o $@

main.o: main.cpp Scanner.h Parser.h Token.h
	$(CC) -c $(CPPFLAGS) $< -o $@

punch: main.o Scanner.o Parser.o
	$(CC) $(CPPFLAGS) $^ -o $@
