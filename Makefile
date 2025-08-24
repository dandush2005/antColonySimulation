# Makefile for Ant Colony Optimization Simulator
# Supports both Windows (with MinGW) and Unix-like systems

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = 

# Platform detection
ifeq ($(OS),Windows_NT)
    # Windows
    EXE_EXT = .exe
    LDFLAGS += -luser32
    RM = del /Q
    MKDIR = mkdir
else
    # Unix-like
    EXE_EXT = 
    LDFLAGS += -lm
    RM = rm -f
    MKDIR = mkdir -p
endif

# Project settings
TARGET = AntColonySimulator$(EXE_EXT)
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(BINDIR)/$(TARGET)

# Create directories
$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Clean build files
clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/$(TARGET)

# Clean everything
distclean: clean
	$(RM) $(OBJDIR)
	$(RM) $(BINDIR)

# Install (copy to system path)
install: $(BINDIR)/$(TARGET)
	cp $(BINDIR)/$(TARGET) /usr/local/bin/

# Run the program
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# Run with test scenario
test: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) --test

# Run test suite
test-suite: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) --test-suite

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the simulator"
	@echo "  clean      - Remove object files"
	@echo "  distclean  - Remove all build files"
	@echo "  install    - Install to system path"
	@echo "  run        - Run the simulator"
	@echo "  test       - Run with test scenario"
	@echo "  test-suite - Run comprehensive test suite"
	@echo "  help       - Show this help"

# Phony targets
.PHONY: all clean distclean install run test help

# Dependencies
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/main.h
$(OBJDIR)/world.o: $(SRCDIR)/world.c $(SRCDIR)/world.h
$(OBJDIR)/ant_logic.o: $(SRCDIR)/ant_logic.c $(SRCDIR)/ant_logic.h
$(OBJDIR)/pheromones.o: $(SRCDIR)/pheromones.c $(SRCDIR)/pheromones.h
$(OBJDIR)/visualization.o: $(SRCDIR)/visualization.c $(SRCDIR)/visualization.h
$(OBJDIR)/file_io.o: $(SRCDIR)/file_io.c $(SRCDIR)/file_io.h
$(OBJDIR)/algorithms.o: $(SRCDIR)/algorithms.c $(SRCDIR)/algorithms.h
$(OBJDIR)/utils.o: $(SRCDIR)/utils.c $(SRCDIR)/utils.h
