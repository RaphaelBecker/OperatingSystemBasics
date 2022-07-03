.PHONY: all run

all: buddy

#############################################################
# DON'T change the variable names of INCLUDEDIRS and SOURCE
#############################################################
# A list of include directories
INCLUDEDIRS =
# A list of source files
SOURCE = buddy.c

# Warning flags
# For more information about gcc warnings: https://embeddedartistry.com/blog/2017/3/7/clang-weverything
# -Wall:		        Print warnings
# -Wextra:		        Enable additional warnings not covered by "-Wall"
# -Wpedantic:	        Reject everything that is not ISO C
# -g					Generates debug information to be used by GDB debugger
# -lm					Includes lib Math during linking for e.g. log2(...)
WFLAGS = -Wall -Wextra -Wpedantic -lm

# Compile without sanitizers and disable optimisation
# $(SOURCE): 	        Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 			        Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:			        Do not optimize the program
# $(WFLAGS)             Warning flags

buddy: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS)

# Compile with address sanitizer enabled
# $(SOURCE): 			Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 					Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:					Do not optimize the program
# $(WFLAGS)             Warning flags
# -Werror:				Treat all warnings as errors
# -fsanitize=address:	Address sanitizer
#                       (https://gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Debugging-Options.html#index-fsanitize_003dundefined-652)
asan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=address

# Compile with undefined behavior sanitizer enabled
# $(SOURCE): 			Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 					Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:					Do not optimize the program
# $(WFLAGS)             Warning flags
# -Werror:				Treat all warnings as errors
# -fsanitize=undefined:	Fast undefined behavior check
#                       (https://gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Debugging-Options.html#index-fsanitize_003dundefined-652)
ubsan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=undefined

# Compile with leak sanitizer enabled
# $(SOURCE): 		    Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 				    Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:				    Do not optimize the program
# $(WFLAGS)             Warning flags
# -Werror:			    Treat all warnings as errors
# -fsanitize=leak	    Basic memory leak sanitizer
#                       (https://gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Debugging-Options.html#index-fsanitize_003dundefined-652)
lsan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=leak

git: # git make m="your message"
	git add .
	git commit -m "$m"
	git push 

# Execute the compiled programm
run:
	./buddy.out

# Make sure we allways rebuild
# Required for the tester
FORCE: ;
