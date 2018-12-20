/* NAME: Ofir Cohen
*  ID: 312255847
*  DATE: 31/12/2018
*
*  This program is an implementation of an http client 
*/

/* INCLUDES */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

/* DEFINES */
#define GET 0
#define POST 1
#define HTTP_PORT 80
#define DEBUG


/* FUNCTIONS */
char* get_params(char* argv[], int starting_params, int ending_params);
int find_port_number(char* url);
char* get_host(char* url);
char* get_path(char* url);
int is_number(char* num);


/* MAIN FUNCTION */
int main(int argc, char* argv[])
{
    /* INITIALIZATION OF HTTP REQUEST */
    int params_flag = -1;
    int path_flag = -1;
    int steps = 0;
    char* url = NULL;


    char* body = NULL;
    char* host = NULL;
    char* params = NULL;
    char* path = NULL;
    int port = HTTP_PORT;
    int request_flag = GET;


    int i;
    for(i = 0; i < argc; i++)
    {
        char* is_url = strstr(argv[i], "http://");

        /* check if GET request or POST request*/        
        if(strcmp(argv[i], "-p") == 0)
        {
            request_flag = POST;
            body = (char*)malloc(strlen(argv[i+1])+1);
            if(body == NULL)
            {
                fprintf(stderr, "Error in malloc");
                exit(1);
            }
            bzero(body, strlen(argv[i+1])+1);
            strcpy(body, argv[i+1]);
        }

        /* check if there are parameters to send */    
        if(strcmp(argv[i], "-r") == 0)
        {
            params_flag = i;
            steps = atoi(argv[i+1]);
            int starting_params = i+2;
            int ending_params = i+2+steps;
            params = get_params(argv, starting_params, ending_params);
        }

        if(is_url && (i < params_flag || i > params_flag + steps + 1 || params_flag == -1))
        {
            url = argv[i];
        }    
    }


    /* GET THE PORT WHERE THE SERVER LISTENS */
    port = find_port_number(url);

    /* GET HOST FROM URL WITHOUT PORT NUMBER */
    host = get_host(url);

    /* GET PATH FROM URL */
    path = get_path(url);
    if(!path)
    {
        path = "/index.html";
        path_flag = 0;
    }


    // /* INITIALIZATION OF SOCKETS */
    // int sockfd;
    // struct hostent* hp;
    // struct sockaddr_in srv;
    
    // /* SETUP CONNECTION TO SERVER */
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if(sockfd < 0)
    // {
    //     perror("Bad fd");
    //     exit(1);
    // }

    // srv.sin_family = AF_INET;
    
    // srv.sin_port = htons(port);
    // hp = gethostbyname(host);

    // if(connect(sockfd, (const struct sockaddr*)&srv, sizeof(srv)) < 0)
    // {
    //     perror("connect");
    //     exit(1);
    // }


    #if defined(DEBUG)
        printf("GET: 0 \r\nPOST: 1\r\n");
        printf("request flag: %d\r\n", request_flag);
        if(request_flag == GET)
            printf("GET REQUEST\r\n");
        else
        {
            printf("POST REQUEST\r\n");
            printf("body: %s\r\n", body);
        }
        printf("params: %s\r\n", params);
        printf("host: %s\r\n", host);
        printf("port: %d\r\n", port);
        printf("path: %s\r\n", path);
    #endif

    if(body)
        free(body);
    
    if(params)
        free(params);

    free(host);

    if(path_flag == -1)
        free(path);

    return 0;    
}


char* get_params(char* argv[], int starting_params, int ending_params)
{
    int sum_of_bytes = 0;

    int i;
    for(i = starting_params; i < ending_params; i++)
    {
        sum_of_bytes += strlen(argv[i]);
    }
    sum_of_bytes += (ending_params - starting_params + 1);
    
    char* params = (char*)malloc(sizeof(char)*sum_of_bytes);
    if(params == NULL)
    {
        return NULL;
    }
    bzero((void*)params, sum_of_bytes);
            
    for(i = starting_params; i < ending_params; i++)
    {
        if(i == starting_params)
        {
            params = strcat(params, "?");
        }

        params = strcat(params, argv[i]);
        
        if(i != ending_params - 1)
        {
            params = strcat(params, "&");            
        }
    }            

    return params;
}


int find_port_number(char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    char* token_host_port;
    char* token_host;
    char* token_port;

    // get rid of http first
    token_host_port = strtok(local_url, "//");

    // get hostname and port without path
    token_host_port = strtok(NULL, "/");

    // get hostname without port number
    token_host = strtok(token_host_port, ":");

    // get port number
    token_port = strtok(NULL, ":");

    //check if a specific port was entered
    if(token_port)
    {
        if(is_number(token_port) == 0)
        {
            free(local_url);
            return atoi(token_port);
        }

        else if(is_number(token_port) == -1)
        {
            free(local_url);
            perror("Error: bad url");
            return -1;
        }
    }

    free(local_url);
    return HTTP_PORT;
}


char* get_host(char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    char* token_host_port;
    char* token_host;

    // get rid of http first
    token_host_port = strtok(local_url, "//");
    
    // get hostname and port
    token_host_port = strtok(NULL, "/");
    
    // get hostname without port number
    token_host = strtok(token_host_port, ":");


    //TODO: need to free in main
    char* host = (char*)malloc(sizeof(char)*(strlen(token_host)+1));
    strcpy(host, token_host);


    free(local_url);
    return host;
}


char* get_path(char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    // get rid of http:
    char* token = strtok(local_url, "//");


    // get only path
    int i;
    for(i = 0; i < 2; i++)
    {
        token = strtok(NULL, "/");
        if(!token)
        {
            free(local_url);
            return NULL;
        }
    }

    //TODO: need to free in main
    char* path = (char*)malloc(sizeof(char)*(strlen(token)+2));
    
    int j;
    for(i = 0, j = 0; i < strlen(token)+2; i++)
    {
        if(i == 0)
            path[i] = '/';
        else if(i == strlen(token)+1)
            path[i] = '\0';
        else
        {
            path[i] = token[j];
            j++;
        }
    }


    free(local_url);
    return path;  
}


int is_number(char* num)
{
    int i;
    for(i = 0; num[i] != '\0'; i++)
    {
        if(num[i] >= '0' && num[i] <= '9')
            continue;
        
        else
            return -1;
    }

    return 0;
}
