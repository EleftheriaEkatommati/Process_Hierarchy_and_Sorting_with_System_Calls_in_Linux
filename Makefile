# Makefile for C project

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror -g

# Source files and header files
SRC_FILES = main.c Processes.c
HEADER_FILES = Processes.h 

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
EXECUTABLE = mysort

# Rule to build the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Rules to build object files
%.o: %.c $(HEADER_FILES)
	$(CC) $(CFLAGS) -c $<

# Clean rule to remove object files and executable
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
