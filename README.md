ofirce
312255847 Ofir Cohen

ex2 - Computer Communication Course

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

client_parse is a struct where we hold host, path, parameters, body of request, port number, the request itself


******************** PROGRAM FUNCTIONS *************************

void get_params(client_parse* client_info, char* argv[], int starting_params, int ending_params);
input: client_parse struct, the array of strings, starting index of parameters, ending index of parameters
output: parameters will be insert to the parameters in the struct


void find_port_number(client_parse* client_info,char* url);
input: client_parse struct, string of url
output: port number will be insert to the port in the struct, if there is no port then will be insert 80
        if the port is invalid then USAGE ERROR / port = -1


void get_host(client_parse* client_info,char* url);
input: client_parse struct, string of url
output: host name will be insert to the url in the struct


void get_path(client_parse* client_info,char* url);
input: client_parse struct, string of url
output: path will be insert to the path in the struct


void create_request(client_parse* client_info, int request_flag);
input: client_parse struct, request_flag (GET or POST)
output: construct the request to be send to the server, will be place in the request in the struct


int is_number(char* num);
input: string of supposed to be number
output: if it is number then return 0, else return -1


void free_client_parse(client_parse* client_info);
input: client_parse struct
output: frees all the content in the struct and the struct itself


int count_digits(int num);
input: the number we want to check how many digits it has
output: the number of digits of the number


int main(int argc, char* argv[]);
input:
output:
