#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"

void send_file(int sd)
{
    int fd;
    struct stat fd_info;
    char *file_ptr;
    int file_size;
    int nbytes;

    fd = open("test.txt", O_RDONLY);
    fstat(fd, &fd_info);
    file_size = fd_info.st_size;

    send(sd, &file_size, sizeof(file_size), 0);

    file_ptr = mmap(NULL, fd_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    while (file_size > 0 && ((nbytes = send(sd, file_ptr, fd_info.st_size, 0) > 0)))
    {
        file_ptr += nbytes;
        file_size -= nbytes;
    }

    munmap(file_ptr, fd_info.st_size);
    close(fd);
}

char *create_directory(void)
{
    char *path;

    if ((path = malloc(sizeof(char) * 256)) == NULL)
        return (NULL);
    getcwd(path, 256);
    ft_strcat(path, "/root");
    mkdir(path, 0777);
    chdir(path);
    return (path);
}

char *do_pwd(void)
{
    return (NULL);
}

/*
char *do_chdir(char *new_path, char *current_path)
{
    // If the path starts with a '/', then current_path can be disregarded since new_path must be an absolute path.
        // Need to verify that the absolute path is within the client's bounds.
    // Else the path doesn't start with a '/' so need to append new_path onto current_path.
    // Then chdir and return the new path to main.
}
*/

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./s port\n");
        exit(-1);
    }

    struct sockaddr_in address, client_info;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t client_info_size;
    int client_socket;

    ft_memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(ft_atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    bind(sd, (struct sockaddr *) &address, sizeof(address));
    
    listen(sd, 5);
    printf("Listening on %s\n", argv[1]);

    client_info_size = sizeof(client_info);
    ft_memset(&client_info, 0, sizeof(client_info));
    
    while ((client_socket = accept(sd, (struct sockaddr *) &client_info, &client_info_size)))
    {
        if (client_socket == -1)
        {
            perror("client socket");
            exit(-1);
        }

        if (fork() == 0)
        {
            printf("Client %s:%d connected.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
            close(sd);
            send_file(client_socket);
            // get_file(client_socket);
            close(client_socket);
            break;
        }
        else
        {
            close(client_socket);
            wait(NULL);
            close(sd);
            break;
        }
    }
}
