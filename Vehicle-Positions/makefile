#-*- makefile-*-
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
all: myapp

myapp: solution.o dist.o
	gcc -Wall -Wenum-conversion -O0 -o myapp solution.o dist.o

solution.o: solution.c 
	gcc -c -Wall -Wenum-conversion -O0 solution.c

dist.o: dist.c 
	gcc -c Wall -Wenum-conversion -O0 dist.c

clean:
	del -i *.o 
	del -i *.exe




