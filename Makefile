PLATFORM ?= PLATFORM_DESKTOP
BUILD_MODE ?= DEBUG
RAYLIB_DIR = C:/raylib
INCLUDE_DIR = -I ./ -I $(RAYLIB_DIR)/raylib/src -I $(RAYLIB_DIR)/raygui/src
LIBRARY_DIR = -L $(RAYLIB_DIR)/raylib/src
DEFINES = -D _DEFAULT_SOURCE -D RAYLIB_BUILD_MODE=$(BUILD_MODE) -D $(PLATFORM)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(findstring Linux,$(PLATFORM_OS)),Linux)
        CC = g++
        CFLAGS = -Wall -std=c++23
        LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
    else
        CC = g++
        EXT = .exe
        ifeq ($(BUILD_MODE),RELEASE)
            CFLAGS ?= $(DEFINES) -D NDEBUG -O3 $(RAYLIB_DIR)/raylib/src/raylib.rc.data $(INCLUDE_DIR) $(LIBRARY_DIR) 
        else
            CFLAGS ?= $(DEFINES) -g $(RAYLIB_DIR)/raylib/src/raylib.rc.data $(INCLUDE_DIR) $(LIBRARY_DIR) 
        endif
        LIBS = $(RAYLIB_DIR)/raylib/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
    endif
endif

SOURCE = $(wildcard src/*.cpp)
HEADER = $(wildcard include/*.h)

.PHONY: all

all: main

main: $(SOURCE) $(HEADER)
	$(CC) -o $@$(EXT) $(SOURCE) $(CFLAGS) $(LIBS) 

clean:
	rm main$(EXT)
