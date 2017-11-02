# PL-0Compiler
A C programming based PL/0 Compiler

README
======

1. To run the program, you must first compile it. For example:

	gcc compiler.c

2. Then you must execute with the appropriate directives in the command terminal, which includes the following.

	-l : print the list of lexemes/tokens (scanner output)

	-a : print the generated assembly code (parser/codegen output)

	-v : print virtual machine execution trace (virtual machine output) 

2a. The program can be executed as the following, or a variation of it:
	
	./a.out -l -a -v

2c. If no flags are give, only the "in" and "out" will be printed to the screen.

3. The program will ask for the source program. Be sure to include .txt at the end of it. For example
	
	source.txt

Misc
====

- The will output information to a text file. The follow information will be outputted.

	* Lexical Analyzer will output lexTokens.txt, which contains the tokens.

	* Parser (-a) will output vmInput.txt, which contains the assembly code for the virtual machine.
		
