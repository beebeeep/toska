TARGET=toska
LIBS=-lncurses
CC=clang
LD=clang
CFLAGS=-Wall -g -Werror -std=c99 -D_POSIX_C_SOURCE=200809L

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

default: $(TARGET)

all: default

%.o: %.c $(HEADERS)
	$(LD) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)


