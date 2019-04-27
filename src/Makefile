CC=g++
CPPFLAGS=-g -Wall -Werror -std=c++17
TARGET=punch

.PHONY: all clean

all: $(TARGET)

# TODO: clean this up properly later

clean:
	rm -f *.o
	rm -f $(TARGET)

%.o: %.cpp %.h
	$(CC) -c $(CPPFLAGS) $< -o $@

Parser.o: Token.h AstNode.h AstProgram.h AstStatement.h AstFunction.h

Scanner.o: Token.h

Translator.o: AstVisitor.h

main.o: Scanner.h Parser.h Translator.h

punch: main.o Scanner.o Parser.o PunchException.o Translator.o
	$(CC) $(CPPFLAGS) $^ -o $@
