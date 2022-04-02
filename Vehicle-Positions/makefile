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

# mode defaults to 'debug' mode.
mode	?= release
prog    := solution
src     := $(wildcard *.c)
objs	:= $(patsubst %.c,%.o,$(src))

# check if mode variable is set for 'debug' or 'release'
# defaults to 'debug' 
ifeq ($(mode), release)
	cflags = -O0 -Wall -Wconversion -std=c11
else
 	mode = debug
 	cflags = -O0 -Wall -g -DDEBUG -Wconversion -std=c11
endif

# link
$(prog): $(objs)
	gcc $(cflags) $(objs) -o $(prog) -lm

# pull in dependency info for *existing* .o files
-include $(objs:.o=.d)

# compile and generate dependency info;
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
%.o: %.c
	gcc -c $(cflags) $*.c -o $*.o
	gcc -MM $(cflags) $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# remove compilation products
clean:
	rm -f $(program) *.o *.d
	