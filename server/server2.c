#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"
#include "common.h"

#define SIZE 256

char g_path[SIZE];


// Doing error checking on client side.


int send_file(int sd, char *command)
{
    int fd;
    struct stat fd_info;
    char *file_ptr;
    int file_size;
    int nbytes;
	char **args;

	args = ft_strsplit(command, ' ');
	if (ft_lstlen(args) != 2)
		return (1);
	// Does this also return -1 if user doesn't have permissions to open the file?
    if ((fd = open(args[1], O_RDONLY)) == -1)
	{
		printf("File does not exist.\n");
		return (1);
	}
    fstat(fd, &fd_info);
    file_size = fd_info.st_size;
    send(sd, &file_size, sizeof(file_size), 0);
    file_ptr = mmap(NULL, fd_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    while (file_size > 0 && ((nbytes = send(sd, file_ptr, fd_info.st_size, 0)) != -1))
    {
        file_ptr += nbytes;
        file_size -= nbytes;
    }
    munmap(file_ptr, fd_info.st_size);
    close(fd);
	printf("%s has been successfully sent.\n", args[1]);
	return (1);
}

int upload_file(int sd, char *command)
{
    int fd;
    int file_size;
    char *buffer;
    int nbytes;
	char *file_name;
	
	if ((file_name = ft_strrchr(command, '/')))
		file_name++;
	else
		file_name = ft_strrchr(command, ' ') + 1;;
    recv(sd, &file_size, sizeof(file_size), 0);
    buffer = malloc(sizeof(char) * file_size);
    fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
    while (file_size > 0 && (nbytes = recv(sd, buffer, 12, 0)) > 0)
    {
        write(fd, buffer, nbytes);
        file_size -= nbytes;
    }
    close(fd);
	printf("%s has successfully downloaded.\n", file_name);
	return (1);
}

void create_directory(void)
{
    getcwd(g_path, sizeof(g_path));
    ft_strcat(g_path, "/root");
    mkdir(g_path, 0777);
    chdir(g_path);
}

int send_pwd(int sd)
{
	send(sd, g_path, ft_strlen(g_path), 0);
	return (1);
}

int change_dir(char *command)
{
	char *path;
	char new_path[SIZE];

	printf("%s\n", command);
	path = ft_strrchr(command, ' ') + 1;
	printf("%s\n", path);
	if (path[0] == '/')
	{
		if (chdir(path) == -1)
		{
			perror("ls: ");
			return (1);
		}
		// Verify permissions (i.e jail).
		ft_memset(&g_path, 0, sizeof(g_path));
		ft_strcpy(g_path, path);
	}
	else
	{
		ft_strcpy(new_path, g_path);
		if (new_path[ft_strlen(new_path) - 1] != '/')
			ft_strcat(new_path, "/");
		ft_strcat(new_path, path);
		if (chdir(new_path) == -1)
		{
			perror("cd: ");
			return (1);
		}
		ft_memset(&g_path, 0, sizeof(g_path));
		ft_strcpy(g_path, new_path);
	}
	printf("Directory has been changed to %s\n", g_path);
	return (1);
}

int send_ls(int socket, char *command)
{
	int fd;
	char **args;

	fd = open("ls", O_RDWR | O_CREAT, 0666); // Error Check
	args = ft_strsplit(command, ' ');
	if (fork() == 0)
	{
		dup2(fd, 1);
		execv("/bin/ls", args);
	}
	else
	{
		wait(NULL); // Change to wait4
		send(socket, NULL, 0, 0);
		close(fd);
	}
	return (1);
}


// If the path starts with a '/', then current_path can be disregarded since new_path must be an absolute path.
        // Need to verify that the absolute path is within the client's bounds.
    // Else the path doesn't start with a '/' so need to append new_path onto current_path.
    // Then chdir and return the new path to main.

int do_op(int socket, char *command)
{
	if (ft_strnequ(command, "ls", 2))
		return (send_ls(socket, command));
	else if (ft_strnequ(command, "cd", 2))
		return (change_dir(command));
	else if (ft_strnequ(command, "pwd", 3))
		return (send_pwd(socket));
	else if (ft_strnequ(command, "get", 3))
		return (send_file(socket, command));
	else if (ft_strnequ(command, "put", 3))
		return (upload_file(socket, command));
	else if (ft_strnequ(command, "quit", 4))
		return (display("User has disconnected.\n", 0));
	return (0);
}

int create_socket(char *port)
{
	int sd;
	int enable;
	struct sockaddr_in address;
	
	sd = socket(AF_INET, SOCK_STREAM, 0);
	enable = 1;
	ft_memset(&address, 0, SOCK_SIZE);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(ft_atoi(port));
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	bind(sd, (struct sockaddr *) &address, sizeof(address));
	listen(sd, 5);
	return (sd);
}

void handle_client(int client, struct sockaddr_in client_info)
{
	char command[256];

	printf("Client %s:%d connected.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

	while (recv(client, command, sizeof(command), 0))
	{
		if (do_op(client, command) == 0)
			break;
		ft_memset(command, 0, sizeof(command));
	}
	close(client);
}

void accept_client(int sd)
{
	int client;
	socklen_t size;
    struct sockaddr_in client_info;
	
	size = sizeof(client_info);
	ft_memset(&client_info, 0, size);
    while ((client = error_check(accept(sd, (struct sockaddr *) &client_info, &size), "client")))
    {
        if (fork() == 0)
        {
			handle_client(client, client_info);
			break;
        }
        else
            close(client);
	}
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./s port\n");
        exit(-1);
    }

	int sd;

	create_directory();
	sd = create_socket(argv[1]); 
	accept_client(sd);
}
