CC = gcc
CFLAGS = -Wall -std=c23
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
SRC = $(wildcard src/*.c)
OUT = main

.PHONY: all run clean $(OUT)

all: $(OUT)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)