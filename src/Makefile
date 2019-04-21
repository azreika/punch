CC=g++
CPPFLAGS=-g -Wall -Werror -std=c++17
TARGET=punch

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

Scanner.o: Scanner.cpp Scanner.h Token.h
	$(CC) -c $(CPPFLAGS) $< -o $@

main.o: main.cpp Token.h
	$(CC) -c $(CPPFLAGS) $< -o $@

punch: main.o Scanner.o
	$(CC) $(CPPFLAGS) $^ -o $@
