/* NAME: Ofir Cohen
*  ID: 312255847
*  DATE: 3/1/2018
*
*  This program is an implementation of an HTTP client 
*/

/* INCLUDES */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


/* DEFINES */
#define GET 0
#define POST 1
#define HTTP_PORT 80
#define SIZE_OF_HTTP 7
#define INDEX_SIZE 2
#define USAGE_ERR "Usage: client [-p <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>\n"
#define BUFFER_SIZE 2


/* PARSER STRUCT */
typedef struct client_parse{
    char* host;
    char* path;
    char* params;
    char* body;
    int port;
    char* request;
}client_parse;


/* FUNCTIONS */
void get_params(client_parse* client_info, char* argv[], int starting_params, int ending_params);
void find_port_number(client_parse* client_info,char* url);
void get_host(client_parse* client_info,char* url);
void get_path(client_parse* client_info,char* url);
void create_request(client_parse* client_info, int request_flag);
int create_socket(client_parse* client_info);
void read_write_from_server(int sockfd, client_parse* client_info);
int is_number(char* num);
void free_client_parse(client_parse* client_info);
int count_digits(int num);


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
    int request_index = -1;     /* checks if the url is inside the body */
    int steps = 0;
    char* url = NULL;

    client_info->host = NULL;
    client_info->path = NULL;
    client_info->params = NULL;
    client_info->body = NULL;
    client_info->request = NULL;
    client_info->port = HTTP_PORT;


    int i, counter = 1;
    for(i = 1; i < argc; i++)
    {
        char* is_url = strstr(argv[i], "http://");

        /* check if GET request or POST request*/        
        if(strcmp(argv[i], "-p") == 0)
        {
            if(i == argc - 1)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            if(request_flag == POST)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            request_index = i;
            request_flag = POST;
            client_info->body = (char*)malloc(strlen(argv[i+1])+1);
            if(client_info->body == NULL)
            {
                perror("malloc");
                exit(1);
            }
            bzero(client_info->body, strlen(argv[i+1])+1);
            strcpy(client_info->body, argv[i+1]);
            counter += 2;
        }


        /* check if there are parameters to send */    
        if(strcmp(argv[i], "-r") == 0)
        {
            if(strcmp(argv[i-1], "-p") == 0)
                continue;
            
            if(params_flag != -1)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            if(i == argc - 1)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            params_flag = i;
            if(is_number(argv[i+1]) == 0)
                steps = atoi(argv[i+1]);
            
            else if(is_number(argv[i+1]) == -1)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            if(steps == 0)
            {
                params_flag = -1;
                counter += 2;
                continue;
            }

            int starting_params = i+2;
            int ending_params = i+2+steps;
            if(ending_params > argc)
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);
            }

            get_params(client_info, argv, starting_params, ending_params);
            counter += steps + 2;
        }

        if(is_url && (i < params_flag || i > params_flag + steps + 1 || params_flag == -1 || request_index == -1 || i < request_index || i > request_index + 2))
        {
            url = argv[i];
            counter++;
        }    
    }

    /* if we didn't get URL or we didn't get any parameters */
    if(counter != argc || url == NULL || (params_flag != -1 && client_info->params == NULL))
    {
        printf(USAGE_ERR);
        free_client_parse(client_info);
        exit(1);
    }

    if(strlen(url) <= SIZE_OF_HTTP)
    {
        printf(USAGE_ERR);
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

    /* INITIALIZATION OF SOCKET */
    int sockfd = create_socket(client_info);
    create_request(client_info, request_flag);

    /* WRITE REQUEST TO SERVER */
    read_write_from_server(sockfd, client_info);
    
    /* FREE STRUCT AND FREE SOCKET DESCRIPTOR */
    free_client_parse(client_info);
    close(sockfd);
    return 0;    
}


/* construct string from the parameters that were entered */
void get_params(client_parse* client_info, char* argv[], int starting_params, int ending_params)
{
    /* get the size of bytes that is needed for the string */
    int sum_of_bytes = 0;
    int i;
    for(i = starting_params; i < ending_params; i++)
    {
        sum_of_bytes += strlen(argv[i]);
    }
    sum_of_bytes += (ending_params - starting_params + 1);
    
    /* contruct the string using malloc */
    client_info->params = (char*)malloc(sizeof(char)*sum_of_bytes);
    if(client_info->params == NULL)
    {
        perror("malloc\r\n");
        return;
    }
    bzero(client_info->params, sum_of_bytes);

    /* concatenate the strings from argv into the parameters string */        
    for(i = starting_params; i < ending_params; i++)
    {
        /* parameters starts with question mark */
        if(i == starting_params)
        {
            client_info->params = strcat(client_info->params, "?");
        }

        /* check if there is a equals sign inside the parameter or it is the first char or last char, if not then Usage error */
        int j, counter = 0;
        for(j = 0; argv[i][j] != '\0'; j++)
        {
            if((j == 0) && (argv[i][j] == '='))
            {
                printf(USAGE_ERR);
                free_client_parse(client_info);
                exit(1);               
            }
           
           if(argv[i][j] == '=')
           {
               counter++;
            }
        }
        if(counter == 0)
        {
            printf(USAGE_ERR);
            free_client_parse(client_info);
            exit(1);
        }

        client_info->params = strcat(client_info->params, argv[i]);
        
        /* between every parameter there is & */
        if(i != ending_params - 1)
        {
            client_info->params = strcat(client_info->params, "&");            
        }
    }          
}


/* gets the port number from the url and copies it to the client_parse struct */
void find_port_number(client_parse* client_info, char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    char* token_host_port;
    char* token_port;

    /* get rid of http first, then get host name without path */
    token_host_port = strtok(local_url, "http://");
    token_host_port = strtok(NULL, "/");

    if(!token_host_port)
    {
        free(local_url);
        client_info->port = HTTP_PORT;
        return;
    }

    /* check if there is specific port number */
    token_port = strchr(token_host_port, ':');


    /* there isn't specific port so the port is 80 */
    if(!token_port)
    {
        free(local_url);
        client_info->port = HTTP_PORT;
        return;
    }


    /* check if no specific port was entered but there are : */ 
    if(strcmp(token_port, ":") == 0)
    {
        free(local_url);
        printf(USAGE_ERR);
        free_client_parse(client_info);
        exit(1);
    }

    /* copy port number to tmp */
    char* tmp = (char*)malloc(sizeof(char)*(strlen(token_port)));
    if(tmp == NULL)
    {
        perror("malloc\r\n");
        return;
    }
    bzero(tmp, sizeof(char)*(strlen(token_port)));
    int i;
    for(i = 1; token_port[i] != '\0'; i++)
    {
        tmp[i-1] = token_port[i];
    }

    
    /* check if a specific port was entered */
    if(tmp)
    {
        /* check is the specific port is a number */
        if(is_number(tmp) == 0)
        {
            free(local_url);
            client_info->port = atoi(tmp);
            free(tmp);
            return;
        }

        /* the specific port is not a number */
        else if(is_number(tmp) == -1)
        {
            // free(local_url);
            // client_info->port = -1;
            // return;
            printf(USAGE_ERR);
            free_client_parse(client_info);
            exit(1);
        }
    }
}


/* gets the host from the url and copies it to the client_parse struct */
void get_host(client_parse* client_info, char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    char* token_host_port;
    char* token_host;

    /* get rid of http first */
    token_host_port = strtok(local_url, "//");
    
    /* get hostname and port */
    token_host_port = strtok(NULL, "/");
    
    /* get hostname without port number */
    token_host = strtok(token_host_port, ":");

    /* copy host to the struct */
    client_info->host = (char*)malloc(sizeof(char)*(strlen(token_host)+1));
    if(client_info->host == NULL)
    {
        perror("malloc\r\n");
        return;
    }
    strcpy(client_info->host, token_host);
    free(local_url);
}


/* gets the path from the url and copies it to the client_parse struct */
void get_path(client_parse* client_info, char* url)
{
    char* local_url = (char*)malloc(sizeof(char)*(strlen(url)+1));
    strcpy(local_url, url);

    /* get rid of http: */
    char* token = strstr(local_url, "http://");
    
    /* get only path */   
    if(strchr(token, '/') == NULL)
    {
        free(local_url);
        return;
    }
    token = strtok(token, "http://");
    token = strtok(NULL, "/");
    token = strtok(NULL, "");
    if(!token)
    {
        free(local_url);
        return;
    }

    /* copy string of path to the struct and add one more place for the / in the start */
    client_info->path = (char*)malloc(sizeof(char)*(strlen(token)+2));
    if(client_info->path == NULL)
    {
        perror("malloc\r\n");
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
}


/* constructs the http request and copies it to the client_parse struct */
void create_request(client_parse* client_info, int request_flag)
{
    /* get the length of each string so we will know what is the size of the request for malloc */
    char* get = "GET ";
    char* post = "POST ";
    char* http = " HTTP/1.0";
    char* host = "Host: ";
    char* content = "Content-length:";
    char* new_line = "\r\n";
    int size = 0;

    /* length of get request and post request are different */
    if(request_flag == GET)
    {
        size += strlen(get);
        size += strlen(new_line);
    }
    else if(request_flag == POST)
    {
        /* we need to know the number of digits of the length of the body for the length of the http request */
        int digits = count_digits(strlen(client_info->body)) + 1;
        char* body_length = (char*)malloc(sizeof(char)*(digits));
        if(body_length == NULL)
        {
            perror("malloc\r\n");
            free_client_parse(client_info);
            exit(1);
        }
        bzero(body_length, sizeof(char)*(digits));
        sprintf(body_length, "%d", (int)strlen(client_info->body));
        
        size += strlen(post);
        size += strlen(content);
        size += strlen(body_length);
        size += strlen(new_line);
        size += strlen(new_line);
        size += strlen(client_info->body);
        size += strlen(new_line);
        free(body_length);
    }

    size += strlen(client_info->path);
    if(client_info->params)
    {
        size += strlen(client_info->params);
    }
    size += strlen(http);
    size += strlen(new_line);
    size += strlen(host);
    size += strlen(client_info->host);
    size += strlen(new_line);

    client_info->request = (char*)malloc(sizeof(char)*(size + 1));
    if(client_info->request == NULL)
    {
        perror("malloc\r\n");
        free_client_parse(client_info);
        exit(1);
    }
    bzero(client_info->request, sizeof(char)*(size + 1));

    /* concatenate the strings to create a request string */
    if(request_flag == GET)
    {
        if(client_info->params)
            sprintf(client_info->request, "GET %s%s HTTP/1.0\r\nHost: %s\r\n\r\n", client_info->path, client_info->params, client_info->host);
        else
            sprintf(client_info->request, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", client_info->path, client_info->host);
    }
    else if(request_flag == POST)
    {
        if(client_info->params)
            sprintf(client_info->request, "POST %s%s HTTP/1.0\r\nHost: %s\r\nContent-length:%d\r\n\r\n%s", client_info->path, client_info->params, client_info->host, (int)strlen(client_info->body), client_info->body);
        else
            sprintf(client_info->request, "POST %s HTTP/1.0\r\nHost: %s\r\nContent-length:%d\r\n\r\n%s", client_info->path, client_info->host, (int)strlen(client_info->body), client_info->body);
    }

    printf("HTTP request =\n%s\nLEN = %d\n", client_info->request, (int)strlen(client_info->request));
}


/* writes HTTP request to server and reads HTTP response from server */
void read_write_from_server(int sockfd, client_parse* client_info)
{
    int sent;
    if((sent = write(sockfd, client_info->request, strlen(client_info->request))) < 0)
    {
        perror("write\r\n");
        exit(1);
    }


    /* READ RESPONSE FROM SERVER while(read(sockfd, buf, sizeof(buf)) > 0) */
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    int sum = 0, nbytes = 0;
    while(1)
    {
        nbytes = read(sockfd, buffer, BUFFER_SIZE - 1);
        if(nbytes == 0)
            break;
        sum += nbytes;
        if(nbytes < 0)
        {
            perror("buffer\r\n");
            exit(1);
        }
        printf("%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }
    printf("\n Total received response bytes: %d\n",sum);
}


/* creates a socket descriptor and connecting to the host */
int create_socket(client_parse* client_info)
{
    int sockfd;
    struct hostent* hp;
    struct sockaddr_in srv;
    
    
    /* SETUP CONNECTION TO SERVER */
    if((hp = gethostbyname(client_info->host)) == NULL)
    {
        herror("gethostbyname\r\n");
        free_client_parse(client_info);
        exit(1);
    }
    bcopy(hp->h_addr, &srv.sin_addr, hp->h_length);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(client_info->port);

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket\r\n");
        exit(1);
    }

    if(connect(sockfd, (const struct sockaddr*)&srv, sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect\r\n");
        exit(1);
    }

    return sockfd;
}

/* checks if a certain string is a number. if it is a number then return 0, else -1 */
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


/* free struct */
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

    if(client_info->request)
        free(client_info->request);

    free(client_info);
}


/* count the number of digits */
int count_digits(int num)
{
    int digits = 0;
    while(num != 0)
    {
        num /= 10;
        digits++;
    }
    return digits;
}
