# Makefile to (1) compile and (2) clean
#  - Rule for compilation should create solution.exe
#     - Should only recompile code when any source code changed
#  - Rule for clean should remove solution.exe
#     - Should not show error if the file does not exist
#  - Use GCC compiler toolchain
#  - Specify the following options:
#     - Disable optimization
#     - Disable generation of debug information
#     - Enable all general warnings
#     - Enable conversion warnings

# compile with gcc for C program
CC = gcc

# compiler flags
# -O0 completely disable most optimisations
# -g0 produces no debug information at all
# -Wall turn on most, but not all compiler warnings
# -Wconversion turn on warn for implicit conversions that may alter a value

#CFLAG += -O0
#CFLAG += -g0
#CFLAGS += -Wall
#CLFAGS += -Wconversion

CFLAGS = -O0 -g0 -Wall -Wconversion

# include math library and pthread library
LIB = -lm -lpthread

EXE = solution
SRC = solution.c binary.c

# build object name from source file
OBJ = $(patsubst %.c,%.o,$(SRC))

all:	$(EXE)

# link all object files with external Libraries into executable/binary
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

# cleans things up
clean:
# RM remove a file, default is 'rm -f'
	$(RM) $(OBJ) core $(EXE)
