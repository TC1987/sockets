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

int get_file(int sd, char *command)
{
    int fd;
    int file_size;
    int nbytes;
    char *buffer;
	char *file_name;

    recv(sd, &file_size, sizeof(file_size), 0);
	if (file_size == -1)
		return (display("Must be a regular file.", 1));
    buffer = malloc(sizeof(char) * file_size);
	file_name = ft_strrchr(command, '/') ? ft_strrchr(command, '/') + 1 : ft_strrchr(command, ' ') + 1;	
    error_check((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)), "open");
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

int send_ls(int sd, char *command)
{
	int fd;
	pid_t pid;
	char **args;

	error_check((fd = open(".ls", O_RDWR | O_TRUNC | O_CREAT, 0666)), "open");
	args = ft_strsplit(command, ' '); // Need to free.
	error_check((pid = fork()), "fork");
	if (pid == 0)
	{
		dup2(fd, 1);
		execv("/bin/ls", args);
	}
	else
	{
		wait4(pid, NULL, 0, NULL);
		send_file_contents(sd, fd);
		unlink(".ls");
	}
	return (1);
}

int send_file(int sd, char *command)
{
    int fd;
	char *file;

	if (ft_word_count(command, ' ') != 2)
		return (1);
	file = ft_strrchr(command, ' ') + 1;
    if ((fd = open(file, O_RDONLY)) == -1)
		return (display("File does not exist or you do not have permissions.", 1));
	send_file_contents(sd, fd);
	printf("%s has been successfully sent.\n", ft_strrchr(file, '/') ? ft_strrchr(file, '/') + 1 : file);
	return (1);
}

int change_dir(int sd, char *command)
{
	char *path;

	path = ft_strrchr(command, ' ') + 1;
	if (chdir(path) == -1)
		ft_strcpy(g_message, "You do not have permissions to this directory or it does not exist.");
	else
	{
		if (!ft_strnequ(g_jail, getcwd(g_path, sizeof(g_path)), ft_strlen(g_jail)))
		{
			ft_strcpy(g_path, g_jail);
			chdir(g_path);
			ft_strcpy(g_message, "You do not have permissions to access this directory.");
		}
		else
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
		return (get_file(sd, command));
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

void serve_clients(int sd)
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
	serve_clients(sd);
}
