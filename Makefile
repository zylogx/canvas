CC = g++
CFLAGS = -Wall -std=c++17
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
SRC = app.cpp main.cpp
OUT = main

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
