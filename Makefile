# Compiler
CC := gcc
CXX := g++

# Directories
SRC_DIR := src
UTIL_DIR := util
BUILD_DIR := build
INCLUDE_DIR := include

# Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
UTIL_FILES := $(wildcard $(UTIL_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)) $(patsubst $(UTIL_DIR)/%.c,$(BUILD_DIR)/%.o,$(UTIL_FILES))

# Flags
CFLAGS := -I$(INCLUDE_DIR) -I/C/glfw-3.4.bin.WIN64/include -I/C/glew-2.1.0/include -I/C/glm -fdiagnostics-color=always
LDFLAGS := -L/C/glew-2.1.0/lib/Release/x64 -L/C/glfw-3.4.bin.WIN64/lib-mingw-w64 -L$(BUILD_DIR)
LIBS := -lglew32 -lglfw3 -lgdi32 -lopengl32 -lws2_32

# Targets
all: app.exe

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CFLAGS) -g -c 