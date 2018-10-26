CC=g++
CPPFLAGS=-Wall -Werror -std=c++11
TARGET=punch

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

main.o: main.cpp
	$(CC) -c $(CPPFLAGS) $^ -o $@

punch: main.o
	$(CC) $(CPPFLAGS) $^ -o $@
