#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

int creat_tcp_IPv4_socket();
struct sockaddr_in *creat_tcp_IPv4_Address(char *ip, int port);
void startlisteningandprintingmessagesonnewthread(int socketFD);
void *listenandprint(void *arg);

void main()
{

    int socketFD = creat_tcp_IPv4_socket();

    struct sockaddr_in *address = creat_tcp_IPv4_Address("127.0.0.1", 2000);

    int result = connect(socketFD, (struct sockaddr *)address, sizeof(*address));

    if (result == 0)
        printf("connection was successful\n");

    char *name = NULL;
    size_t namesize = 0;
    printf("Please enter your name ....\n");
    ssize_t nameCount = getline(&name, &namesize, stdin);
    name[nameCount - 1] = 0;

    char *line = NULL;
    size_t linesize = 0;
    printf("Ask the server ....\n");

    startlisteningandprintingmessagesonnewthread(socketFD);
    char* buffer = (char*)malloc(1024);
    while (true)
    {
        ssize_t charCount = getline(&line, &linesize, stdin);
        line[charCount - 1] = 0;
        sprintf(buffer, "%s:%s", name, line);

        if (charCount > 0)
        {
            if (strcmp(line, "exit") == 0)
            {
                free (buffer);
                break;
            }
        }
        ssize_t amount_was_sent = send(socketFD, buffer, strlen(buffer), 0);
    }

    close(socketFD);
}

int creat_tcp_IPv4_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in *creat_tcp_IPv4_Address(char *ip, int port)
{

    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    return address;
}

void startlisteningandprintingmessagesonnewthread(int socketFD)
{
    pthread_t id;
    int *socketFDPtr = malloc(sizeof(int));
    *socketFDPtr = socketFD;
    pthread_create(&id, NULL, listenandprint, socketFDPtr);
}

void *listenandprint(void *arg)
{
    int socketFD = *(int *)arg;
    char buffer[1024];

    while (true)
    {
        ssize_t amountreceived = recv(socketFD, buffer, 1024, 0);

        if (amountreceived > 0)
        {
            buffer[amountreceived] = 0;
            printf("%s\n", buffer);
        }
        if (amountreceived == 0)
            break;
    }
    close(socketFD);
}
