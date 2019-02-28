#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"

int ft_lstlen(char **list)
{
	int i;

	i = 0;
	if (!list || !(*list))
		return (0);
	while (*list)
	{
		i++;
		list++;
	}
	return (i);
}

char *get_full_path(char *current_path, char *path_to_add)
{
	char *full_path;

	if (path_to_add[0] == '/')
		return (path_to_add);
	full_path = ft_strnew(ft_strlen(current_path) + ft_strlen(path_to_add) + 2);
	full_path = ft_strcat(full_path, current_path);
	if (full_path[ft_strlen(current_path) - 1] != '/')
		ft_strcat(full_path, "/");
	ft_strcat(full_path, path_to_add);
	return (full_path);
}

int send_file(int sd, char *command, char *path)
{
    int fd;
    struct stat fd_info;
    char *file_ptr;
    int file_size;
    int nbytes;
	char **args;
	char *full_path;

	args = ft_strsplit(command, ' ');
	if (ft_lstlen(args) != 2)
		return (0);

	full_path = get_full_path(path, args[1]);
	printf("full_path: %s\n", full_path);

    fd = open(full_path, O_RDONLY);
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
	return (1);
}

char *create_directory(void)
{
    char *path;
	char *full_path;

    if ((path = malloc(sizeof(char) * 256)) == NULL)
        return (NULL);
    path = getcwd(NULL, 0);
    full_path = ft_strjoin(path, "/root");
    mkdir(full_path, 0777);
    chdir(full_path);
	free(path);
    return (full_path);
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

int do_op(int socket, char *command, char *path)
{
	if (ft_strnequ(command, "ls", 2))
	{	
		execl("/bin/ls", "ls");
	}
	else if (ft_strnequ(command, "cd", 2))
	{
	}
	else if (ft_strnequ(command, "pwd", 3))
	{
	}
	else if (ft_strnequ(command, "get", 3))
	{
		return (send_file(socket, command, path));
	}
	else if (ft_strnequ(command, "put", 3))
	{
	}
	else if (ft_strnequ(command, "quit", 4))
	{
		return (0);
	}
	return (1);
}

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
	char *path;
	
	path = create_directory();

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
			
			int keep_alive = 1;
			char command[256];

			while (keep_alive)
			{
				close(sd);
				recv(client_socket, &command, sizeof(command), 0); 
				if ((keep_alive = do_op(client_socket, command, path)) == 0)
					break;
			}
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
