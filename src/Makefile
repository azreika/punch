CC=g++
CPPFLAGS=-g -Wall -Werror -std=c++17
TARGET=punch

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

%.o: %.cpp %.h
	$(CC) -c $(CPPFLAGS) $< -o $@

Parser.o: Token.h AstNode.h AstProgram.h AstStatement.h

Scanner.o: Token.h

main.o: Scanner.h Parser.h Token.h

punch: main.o Scanner.o Parser.o PunchException.o
	$(CC) $(CPPFLAGS) $^ -o $@
