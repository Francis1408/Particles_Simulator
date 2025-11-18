# Makefile for GLFW + GL project with glad

# Compiler and flags
CXX = g++
CFLAGS = -fdiagnostics-color=always -g

# Directories
SRC_DIR = .
GLAD_DIR = glad

# Source files
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
GLAD_FILES = $(wildcard $(GLAD_DIR)/*.c)

# Output binary
OUT = main

# Use pkg-config to get FreeType flags
FREETYPE_CFLAGS = $(shell pkg-config --cflags freetype2)
FREETYPE_LIBS = $(shell pkg-config --libs freetype2)

# Libraries
LIBS = -lGL -lglfw $(FREETYPE_LIBS)

# Build target
all: $(OUT)

$(OUT): $(CPP_FILES) $(GLAD_FILES)
	$(CXX) $(CFLAGS) $(FREETYPE_CFLAGS) $^ -o $@ $(LIBS)

# Run target
run: $(OUT)
	./$(OUT) $(ARGS)

# Clean target
clean:
	rm -f $(OUT)