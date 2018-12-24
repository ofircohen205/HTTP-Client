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
#define INDEX_SIZE 12
#define DEBUG


/* PARSER STRUCT */
typedef struct client_parse{
    char* host;
    char* path;
    char* params;
    char* body;
    int port;
}client_parse;


/* FUNCTIONS */
void get_params(client_parse* client_info, char* argv[], int starting_params, int ending_params);
void find_port_number(client_parse* client_info,char* url);
void get_host(client_parse* client_info,char* url);
void get_path(client_parse* client_info,char* url);
char* create_request(client_parse* client_info, int request_flag);
int is_number(char* num);
void free_client_parse(client_parse* client_info);


/* MAIN FUNCTION */
int main(int argc, char* argv[])
{
    /* INITIALIZATION OF HTTP REQUEST */
    client_parse *client_info = (client_parse*)malloc(sizeof(client_parse));
    if(client_info == NULL)
    {
        perror("client_parse struct\r\n");
        exit(1);
    }

    int params_flag = -1;
    int request_flag = GET;
    int steps = 0;
    char* url = NULL;


    // char* body = NULL;
    // char* host = NULL;
    // char* params = NULL;
    // char* path = NULL;
    // int port = HTTP_PORT;
    client_info->host = NULL;
    client_info->path = NULL;
    client_info->params = NULL;
    client_info->body = NULL;
    client_info->port = HTTP_PORT;


    int i;
    for(i = 0; i < argc; i++)
    {
        char* is_url = strstr(argv[i], "http://");

        /* check if GET request or POST request*/        
        if(strcmp(argv[i], "-p") == 0)
        {
            request_flag = POST;
            client_info->body = (char*)malloc(strlen(argv[i+1])+1);
            if(client_info->body == NULL)
            {
                fprintf(stderr, "Error in malloc");
                exit(1);
            }
            bzero(client_info->body, strlen(argv[i+1])+1);
            strcpy(client_info->body, argv[i+1]);
        }

        /* check if there are parameters to send */    
        if(strcmp(argv[i], "-r") == 0)
        {
            params_flag = i;
            if(is_number(argv[i+1]) == 0)
                steps = atoi(argv[i+1]);
            //TODO: write down correctly perror
            else
            {
                perror("error in number of params");
                exit(1);
            }
            int starting_params = i+2;
            int ending_params = i+2+steps;
            get_params(client_info, argv, starting_params, ending_params);
        }

        if(is_url && (i < params_flag || i > params_flag + steps + 1 || params_flag == -1))
        {
            url = argv[i];
        }    
    }

    if(!url || (params_flag != -1 && client_info->params == NULL))
    {
        perror("error no url was sent or bad params");
        free_client_parse(client_info);
        exit(1);
    }


    /* GET THE PORT WHERE THE SERVER LISTENS */
    find_port_number(client_info, url);

    /* GET HOST FROM URL WITHOUT PORT NUMBER */
    get_host(client_info, url);

    /* GET PATH FROM URL */
    get_path(client_info, url);
    if(client_info->path == NULL)
    {
        client_info->path = (char*)malloc(sizeof(char)*INDEX_SIZE);
        strcpy(client_info->path, "/");
    }


    // /* INITIALIZATION OF SOCKETS */
    // int sockfd;
    // struct hostent* hp;
    // struct sockaddr_in srv;
    
    // /* SETUP CONNECTION TO SERVER */
    // if((hp = gethostbyname(client_info->host)) == NULL)
    // {
    //     herror("gethostbyname\r\n");
    //     exit(1);
    // }
    // bcopy(hp->h_addr, &srv.sin_addr, hp->h_length);
    // srv.sin_family = AF_INET;
    // srv.sin_port = htons(client_info->port);

    // if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     perror("socket\r\n");
    //     exit(1);
    // }

    // if(connect(sockfd, (const struct sockaddr*)&srv, sizeof(struct sockaddr_in)) < 0)
    // {
    //     perror("connect\r\n");
    //     exit(1);
    // }

    // /* WRITE REQUEST TO SERVER WITH write syscall */
    char* request = create_request(client_info, request_flag);
    

    // /* READ RESPONSE FROM SERVER while(read(sockfd, buf, sizeof(buf)) > 0) */
    // char buffer[1024];
    // bzero(buffer, strlen(buffer));

    // int sum = 0, nbytes;
    // while(1)
    // {
    //     nbytes = read(sockfd, buffer + sum, strlen(buffer) - sum);
    //     if(nbytes == 0)
    //         break;
    //     sum += nbytes;
    //     if(nbytes < 0)
    //     {
    //         perror("buffer\r\n");
    //         exit(1);
    //     }
    // }

    #if defined(DEBUG)
        printf("GET: 0 \r\nPOST: 1\r\n");
        printf("request flag: %d\r\n", request_flag);
        if(request_flag == GET)
            printf("GET REQUEST\r\n");
        else
        {
            printf("POST REQUEST\r\n");
            printf("body: %s\r\n", client_info->body);
        }
        printf("params: %s\r\n", client_info->params);
        printf("host: %s\r\n", client_info->host);
        printf("port: %d\r\n", client_info->port);
        printf("path: %s\r\n\r\n", client_info->path);
        printf("request:\r\n %s", request);
    #endif

    free_client_parse(client_info);
    free(request);
    //close(sockfd);
    return 0;    
}


void get_params(client_parse* client_info, char* argv[], int starting_params, int ending_params)
{
    int sum_of_bytes = 0;

    int i;
    for(i = starting_params; i < ending_params; i++)
    {
        sum_of_bytes += strlen(argv[i]);
    }
    sum_of_bytes += (ending_params - starting_params + 1);
    
    client_info->params = (char*)malloc(sizeof(char)*sum_of_bytes);
    if(client_info->params == NULL)
    {
        herror("error on params\r\n");
        return;
    }
    bzero((void*)client_info->params, sum_of_bytes);
            
    char* tmp;
    for(i = starting_params; i < ending_params; i++)
    {
        if(i == starting_params)
        {
            client_info->params = strcat(client_info->params, "?");
        }

        tmp = strchr(argv[i], '=');
        if(tmp == NULL)
        {
            //TODO: fix correctly the perror
            free_client_parse(client_info);
            perror("usage:\r\n");
            exit(1);
        }
        client_info->params = strcat(client_info->params, argv[i]);
        
        if(i != ending_params - 1)
        {
            client_info->params = strcat(client_info->params, "&");            
        }
    }          
}


void find_port_number(client_parse* client_info, char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    char* token_host_port;
    char* token_port;

    // get rid of http first
    token_host_port = strtok(local_url, "http://");
    token_host_port = strtok(NULL, "/");

    token_port = strchr(token_host_port, ':');

    if(!token_port)
    {
        free(local_url);
        client_info->port = HTTP_PORT;
        return;
    }
    
    char* tmp = (char*)malloc(sizeof(char)*(strlen(token_port)));
    if(tmp == NULL)
    {
        herror("tmp is NULL\r\n");
        return;
    }
    
    int i;
    for(i = 1; token_port[i] != '\0'; i++)
    {
        tmp[i-1] = token_port[i];
    }

    
    //check if a specific port was entered
    if(tmp)
    {
        if(is_number(tmp) == 0)
        {
            free(local_url);
            client_info->port = atoi(tmp);
            free(tmp);
            return;
        }

        else if(is_number(tmp) == -1)
        {
            free(local_url);
            herror("Error: bad url");
            return;
        }
    }
}


void get_host(client_parse* client_info, char* url)
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
    client_info->host = (char*)malloc(sizeof(char)*(strlen(token_host)+1));
    if(client_info->host == NULL)
    {
        herror("client_info->host is NULL\r\n");
        return;
    }
    strcpy(client_info->host, token_host);

    free(local_url);
    return;
}


void get_path(client_parse* client_info, char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    // get rid of http:
    char* token = strtok(local_url, "http://");
    
    // get only path    
    token = strtok(NULL, "/");
    token = strtok(NULL, "");

    //TODO: need to free in main
    client_info->path = (char*)malloc(sizeof(char)*(strlen(token)+2));
    if(client_info->path == NULL)
    {
        herror("client_info->path is NULL\r\n");
        return;
    }

    int i, j;
    for(i = 0, j = 0; i < strlen(token)+2; i++)
    {
        if(i == 0)
            client_info->path[i] = '/';
        else if(i == strlen(token)+1)
            client_info->path[i] = '\0';
        else
        {
            client_info->path[i] = token[j];
            j++;
        }
    }

    free(local_url);
    return;  
}


char* create_request(client_parse* client_info, int request_flag)
{
    char* request = NULL;
    // char* get = "GET ";
    // char* post = "POST ";
    // char* http = " HTTP/1.0";
    // char* host = "Host: ";
    // char* new_line = "\r\n";
    int size = 0;
    size += strlen(" HTTP/1.0\r\nHost: ") + strlen("\r\n\r\n") + strlen(client_info->path) + strlen(client_info->host) + strlen(client_info->params);
    if(request_flag == GET)
    {
        size += strlen("GET ");
        request = (char*)malloc(sizeof(char)*(size + 1));
        bzero(request, strlen(request));
        sprintf(request, "GET %s%s HTTP/1.0\r\nHost: %s\r\n\r\n", client_info->path, client_info->params, client_info->host);
    }
    else if(request_flag == POST)
    {
        size += strlen(client_info->body) + strlen("POST ") + strlen("\r\n");
        request = (char*)malloc(sizeof(char)*(size+1));
        bzero(request, strlen(request));
        sprintf(request, "POST %s%s HTTP/1.0\r\nHost: %s\r\nContent-length:%d\r\n\r\n%s\r\n", client_info->path, client_info->params, client_info->host, (int)strlen(client_info->body), client_info->body);
    }
    return request; 
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


void free_client_parse(client_parse* client_info)
{
    if(client_info->host)
        free(client_info->host);

    if(client_info->params)
        free(client_info->params);

    if(client_info->path)
        free(client_info->path);

    if(client_info->body)
        free(client_info->body);


    free(client_info);
}
