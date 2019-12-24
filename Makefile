.PHONY: all run

all: main

#############################################################
# DON'T change the variable names of INCLUDEDIRS and SOURCE
#############################################################
# A list of include directories
INCLUDEDIRS =
# A list of source files
SOURCE = main.c

# Warning flags
# For more information about gcc warnings: https://embeddedartistry.com/blog/2017/3/7/clang-weverything
# -Wall:		        Print warnings
# -Wextra:		        Enable additional warnings not covered by "-Wall"
# -Wpedantic:	        Reject everything that is not ISO C
# -g					Generates debug information to be used by GDB debugger
WFLAGS = -Wall -Wextra -Wpedantic -g

# Compile without sanitizers and disable optimisation
# $(SOURCE): 	        Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 			        Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:			        Do not optimize the program
# $(WFLAGS)             Warning flags
# -pthread: 	        Tell the system we want to use the pthread library
main: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -pthread

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
# -pthread: 	        Tell the system we want to use the pthread library
asan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=address -pthread

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
# -pthread: 	        Tell the system we want to use the pthread library
ubsan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=undefined -pthread

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
# -pthread: 	        Tell the system we want to use the pthread library
lsan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=leak -pthread

# Compile with thread sanitizer enabled
# $(SOURCE): 		    Input file(s)
# $(INCLUDEDIRS:%=-I%)  Include directories
# -o: 				    Link the resulting object files
# $@.out:	            Built-in variable representing the current target name + file extension .out
# -std=c11              Set C standard
# -O0:				    Do not optimize the program
# $(WFLAGS)             Warning flags
# -Werror:			    Treat all warnings as errors
# -pthread: 	        Tell the system we want to use the pthread library
#                       (https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)
# -fsanitize=thread	    Thread sanitizer
tsan: FORCE
	gcc $(SOURCE) $(INCLUDEDIRS:%=-I%) -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=thread -pthread

# Execute the compiled programm
run:
	./main.out

# Make sure we allways rebuild
# Required for the tester
FORCE: ;
