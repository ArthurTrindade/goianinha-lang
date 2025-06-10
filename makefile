# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -g

# Directory structure
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Target executable name
TARGET = goianinha

# Flex (Lexer) files
LEX_FILE = $(SRC_DIR)/goianinha.l
LEX_C = $(SRC_DIR)/lex.yy.c
LEX_OBJ = $(BUILD_DIR)/lex.yy.o

# Bison (Parser) files
BISON_FILE = $(SRC_DIR)/goianinha.y
BISON_C = $(SRC_DIR)/goianinha.tab.c
BISON_H = $(SRC_DIR)/goianinha.tab.h
BISON_OBJ = $(BUILD_DIR)/goianinha.tab.o

# Source files
SRC_FILES = $(SRC_DIR)/list.c \
            $(SRC_DIR)/ast.c \
            $(SRC_DIR)/symbol_table.c \
            $(SRC_DIR)/hashmap.c \
			$(SRC_DIR)/print_ast.c

# Generate object file paths from source files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJ_FILES += $(LEX_OBJ) $(BISON_OBJ)

# Include directories
INCLUDES = -I$(INCLUDE_DIR) -I$(SRC_DIR)

# Default target
.PHONY: all
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ_FILES)
	@echo "Linking $@..."
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful! Binary created: $(TARGET)"

# Generate parser from bison file
$(BISON_C) $(BISON_H): $(BISON_FILE) | $(BUILD_DIR)
	@echo "Generating parser..."
	@bison -d -o $(BISON_C) $<

# Generate lexer from flex file (depends on bison output for header)
$(LEX_C): $(LEX_FILE) $(BISON_H) | $(BUILD_DIR)
	@echo "Generating lexical analyzer..."
	@flex -o $@ $<

# Compile bison source to object file
$(BISON_OBJ): $(BISON_C)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Compile lexer source to object file
$(LEX_OBJ): $(LEX_C)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Compile C source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@echo "Creating build directory..."
	@mkdir -p $(BUILD_DIR)

# Clean up build artifacts
.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) $(TARGET) $(LEX_C) $(BISON_C) $(BISON_H)
	@echo "Clean complete!"

# Run the compiled program
.PHONY: run
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all       - Build the executable (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the executable"
	@echo "  help      - Display this help message"
