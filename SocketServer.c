#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

struct AcceptedSocket
{
    int acceptedsocketFD;
    struct sockaddr_in address;
    bool acceptedsuccessfully;
    int error;
};

int creat_tcp_IPv4_socket();
struct sockaddr_in *creat_tcp_IPv4_Address(char *ip, int port);
struct AcceptedSocket *accept_incoming_connections(int serversocketFD);
void *receiveandprintincomingdata(void *arg);
void startacceptingincomingconnection(int serversocketFD);
void acceptnewconnectionrecevingandprinting(int serversocketFD);
void receiveandprintinthread(struct AcceptedSocket *pSocket);
struct AcceptedSocket acceptedsockets[10];
int acceptedsocketcount = 0;
void sendreceivemessagebroadcasting(char *buffer, int socketFD);

void main()
{

    int serversocketFD = creat_tcp_IPv4_socket();
    struct sockaddr_in *ServerAddress = creat_tcp_IPv4_Address("", 2000);

    int result = bind(serversocketFD, (struct sockaddr *)ServerAddress, sizeof(*ServerAddress));
    if (result == 0)
        printf("socket was bound successfully\n");

    int res = listen(serversocketFD, 10);
    if (res == 0)
        printf("socket was listened successfully\n");

    startacceptingincomingconnection(serversocketFD);
    shutdown(serversocketFD, SHUT_RDWR);
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
    if (strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    return address;
}

struct AcceptedSocket *accept_incoming_connections(int serversocketFD)
{
    struct sockaddr_in ClientAddress;
    int ClientAddressSize = sizeof(struct sockaddr_in);
    int clientsocketFD = accept(serversocketFD, (struct sockaddr *)&ClientAddress, &ClientAddressSize);

    struct AcceptedSocket *acceptsocket = malloc(sizeof(struct AcceptedSocket));
    acceptsocket->address = ClientAddress;
    acceptsocket->acceptedsocketFD = clientsocketFD;
    acceptsocket->acceptedsuccessfully = clientsocketFD > 0;
    if (!acceptsocket->acceptedsuccessfully)
        acceptsocket->error = clientsocketFD;
    return acceptsocket;
}

void *receiveandprintincomingdata(void *arg)
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
            sendreceivemessagebroadcasting(buffer, socketFD);
        }

        if (amountreceived == 0)
        {
            break;
        }
    }
    close(socketFD);
}

void startacceptingincomingconnection(int serversocketFD)
{
    while (true)
    {
        struct AcceptedSocket *clientsocket = accept_incoming_connections(serversocketFD);
        acceptedsockets[acceptedsocketcount++] = *clientsocket;
        receiveandprintinthread(clientsocket);
    }
}

void receiveandprintinthread(struct AcceptedSocket *pSocket)
{
    pthread_t id;
    int *socketFD = malloc(sizeof(int));
    *socketFD = (pSocket->acceptedsocketFD);
    pthread_create(&id, NULL, receiveandprintincomingdata, socketFD);
}

void sendreceivemessagebroadcasting(char *buffer, int socketFD)
{
    for (int i = 0; i < acceptedsocketcount; i++)
        if (acceptedsockets[i].acceptedsocketFD != socketFD)
        {
            send(acceptedsockets[i].acceptedsocketFD, buffer, strlen(buffer), 0);
        }
}
