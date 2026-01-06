# Makefile for GLFW + GL project with glad

# Compiler and flags
CXX = g++
CFLAGS = -fdiagnostics-color=always -g

# Directories
SRC_DIR = src
UTILS_DIR = src/utils
GLAD_DIR = glad
BUILD_DIR = build

# Source files
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp) \
			$(wildcard $(UTILS_DIR)/*.cpp)
GLAD_FILES = $(wildcard $(GLAD_DIR)/*.c)


# Tell make where to find source files
vpath %.cpp $(SRC_DIR) $(UTILS_DIR)
vpath %.c   $(GLAD_DIR)

# Object files (flattened into build/)
OBJ_CPP  = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(CPP_FILES)))
OBJ_GLAD = $(patsubst %.c,  $(BUILD_DIR)/%.o, $(notdir $(GLAD_FILES)))

OBJS = $(OBJ_CPP) $(OBJ_GLAD)

# Output binary
OUT = main

# Use pkg-config to get FreeType flags
FREETYPE_CFLAGS = $(shell pkg-config --cflags freetype2)
FREETYPE_LIBS = $(shell pkg-config --libs freetype2)

# Libraries
LIBS = -lGL -lglfw $(FREETYPE_LIBS)

# Build target
all: $(OUT)

# Link step
$(OUT): $(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

# Compile C++ files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(FREETYPE_CFLAGS) -c $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

# Run target
run: $(OUT)
	./$(OUT) $(ARGS)

# Clean target
clean:
	rm -rf $(BUILD_DIR) $(OUT)