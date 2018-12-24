ofirce
312255847 Ofir Cohen

ex1 - Computer Communication Course

This program implements HTTP Client request and response

list of files:
client.c
README.md

how to install the program:
open linux terminal, navigate to the folder containing ex1
using the "cd" command (confirm it by using ls command)
incase you have makefile, type make and the program will
automaticily be compiled, if you don't, type 
gcc client.c -o client -Wall

and your program will automaticily be compiled

to activate program:
open linux terminal, navigate to ex2 executeable file
location using "cd" command (confirm it using ls command) and type
valgrind ./client client [-p <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>
when -p and -r are optional