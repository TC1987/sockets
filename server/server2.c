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
char g_jail[SIZE];
char g_message[4096];

// Doing error checking on client side.

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
	ft_memcpy(g_jail, g_path, sizeof(g_path));
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

int send_file(int sd, char *command)
{
    int fd;
    struct stat fd_info;
    char *file_ptr;
    int file_size;
    int nbytes;
	char *file;

	if (ft_word_count(command, ' ') != 2)
		return (1);
	file = ft_strrchr(command, ' ') + 1;
	// Does this also return -1 if user doesn't have permissions to open the file?
    if ((fd = open(file, O_RDONLY)) == -1)
		return (display("File does not exist.\n", 1));
    fstat(fd, &fd_info);
	if (!S_ISREG(fd_info.st_mode))
	{
		file_size = -1;
		send(sd, &file_size, sizeof(file_size), 0);
		return (display("Not a file", 1));
	}
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
	printf("%s has been successfully sent.\n", file);
	return (1);
}

int change_dir(int sd, char *command)
{
	char *path;

	path = ft_strrchr(command, ' ') + 1;
	if (*path == '/')
	{
		if (!ft_strnequ(g_jail, path, sizeof(g_jail)))
			ft_strcpy(g_message, "Do not have permission.");
	}
	else if (ft_strnequ(path, "..", 2))
	{
		
	}
	if (chdir(path) == -1)
		ft_strcpy(g_message, "Path does not exist.");
	else
	{
		getcwd(g_path, sizeof(g_path));
		printf("Directory has been changed to %s\n", g_path);
		ft_strcpy(g_message, g_path);
	}
	send(sd, g_message, sizeof(g_message), 0);
	return (1);
}

int send_pwd(int sd)
{
	send(sd, g_path, ft_strlen(g_path), 0);
	return (1);
}

int do_op(int sd, char *command)
{
	if (ft_strnequ(command, "ls", 2))
		return (send_ls(sd, command));
	else if (ft_strnequ(command, "cd", 2))
		return (change_dir(sd, command));
	else if (ft_strnequ(command, "pwd", 3))
		return (send_pwd(sd));
	else if (ft_strnequ(command, "get", 3))
		return (send_file(sd, command));
	else if (ft_strnequ(command, "put", 3))
		return (upload_file(sd, command));
	else if (ft_strnequ(command, "quit", 4))
		return (display("User has disconnected.", 0));
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
	
	ft_memset(command, 0, sizeof(command));
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
			return;
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
