#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "libft.h"

void get_file(int sd)
{
    int fd;
    int file_size;
    char *buffer;
    int nbytes;

    printf("Getting test_received.txt.\n");

    recv(sd, &file_size, sizeof(file_size), 0);

    buffer = malloc(sizeof(char) * file_size);
    fd = open("test_received.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);

    while ((nbytes = recv(sd, buffer, file_size, 0)) > 0)
    {
        write(fd, buffer, nbytes);
        file_size -= nbytes;
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./c ip port\n");
        exit(-1);
    }

    struct sockaddr_in address;
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    ft_memset(&address, 0, sizeof(address));
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(ft_atoi(argv[2]));
    address.sin_family = AF_INET;

    if (connect(sd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        perror("connect");
        exit(-1);
    }

    printf("Connected to server.\n");

    get_file(sd);

    // send_file(sd);

    close(sd);
}
