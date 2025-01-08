CC = g++
CFLAGS = -Wall -std=c++23
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
SRC = $(wildcard src/*.cpp)
OUT = main

.PHONY: all run clean $(OUT)

all: $(OUT)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)