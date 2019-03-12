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
	int remaining;
    int file_size;
    int nbytes;
    char *buffer;
	char *file_name;

    recv(sd, &file_size, sizeof(file_size), 0);
	if (file_size == -1)
		return (display("Must be a valid file.", 1));
	remaining = file_size;
    buffer = malloc(sizeof(char) * file_size);
	file_name = ft_strrchr(command, '/') ? ft_strrchr(command, '/') + 1 : ft_strrchr(command, ' ') + 1;
    error_check((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)), "open");
    while (remaining > 0 && (nbytes = recv(sd, buffer, file_size, 0)) > 0)
    {
		printf("%d / %d received\n", nbytes, file_size);
        write(fd, buffer, nbytes);
        remaining -= nbytes;
    }
    close(fd);
	printf("%s has successfully downloaded.\n", file_name);
	free(buffer);
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

	error_check((fd = open(".ls", O_RDWR | O_TRUNC | O_CREAT, 0777)), "open");
	args = ft_strsplit(command, ' ');
	error_check((pid = fork()), "fork");
	if (pid == 0)
	{
		dup2(fd, 1);
		dup2(fd, 2);
		execv("/bin/ls", args);
	}
	else
	{
		wait4(pid, NULL, 0, NULL);
		send_file_contents(sd, fd);
		unlink(".ls");
		free_list(args);
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
    fd = open(file, O_RDONLY);
	if (send_file_contents(sd, fd))
		printf("%s has been successfully sent.\n", ft_strrchr(file, '/') ? ft_strrchr(file, '/') + 1 : file);
	return (1);
}

int change_dir(int sd, char *command)
{
	char *path;
	char *current_path;

	path = ft_strrchr(command, ' ') + 1;
	current_path = ft_strdup(g_path);
	ft_strcat(g_path, "/");
	ft_strcat(g_path, path);
	if (chdir(g_path) == -1 || !ft_strnequ(g_jail, getcwd(g_path, sizeof(g_path)), ft_strlen(g_jail)))
	{
		ft_strcpy(g_path, current_path);
		chdir(g_path);
		ft_strcpy(g_message, "Directory does not exist or you do not have permissions to access this directory.");
	}
	else
		ft_strcpy(g_message, g_path);
	send(sd, g_message, sizeof(g_message), 0);
	free(current_path);
	return (1);
}

int send_pwd(int sd)
{
	send(sd, g_path, ft_strlen(g_path), 0);
	return (1);
}

int check_dir(char *path)
{
	int i;
	int status;
	char *dir;
	char *current_path;

	i = 0;
	status = 1;
	if ((dir = ft_strrchr(path, '/')) == NULL)
		return (1);
	while (&path[i] != dir)
		i++;
	dir = ft_strndup(path, i);
	current_path = ft_strdup(g_path);
	if (chdir(dir) == -1 || (!ft_strnequ(g_jail, getcwd(g_path, sizeof(g_path)), ft_strlen(g_jail))))
	{
		status = 0;
		ft_strcpy(g_path, current_path);
		chdir(g_path);
	}
	free(dir);
	free(current_path);
	return (status);	
}

int do_rm(int sd, char *command)
{
	char *file;

	file = ft_strrchr(command, ' ') + 1;
	if (*file == '/')
	{
		if (!ft_strnequ(g_jail, file, ft_strlen(g_jail)))
			ft_strcpy(g_message, "The file you're trying to remove exists outside of your working directory.");
		else if (unlink(file) == -1)
			ft_strcpy(g_message, "There was an error removing the file; the file does not exist.");
		else
		{
			ft_strcpy(g_message, file);
			ft_strcat(g_message, " has been successfully removed.");
		}
	}	
	else
	{
		if (!check_dir(file))
			ft_strcpy(g_message, "The file you're trying to remove exists outside of your working directory.");
		if (unlink(file) == -1)
			ft_strcpy(g_message, "The file does not exist or you do not have permissions.");
		ft_strcpy(g_message, "The file has been deleted.");
	}
	send(sd, g_message, sizeof(g_message), 0);
	return (1);
}

int do_mkdir(int sd, char *command)
{
	char *dir;
	char *folder_name;

	dir = ft_strrchr(command, ' ') + 1;
	if (!check_dir(dir))
		ft_strcpy(g_message, "You are trying to create a directory outside of your working directory.");
	folder_name = ft_strrchr(dir, '/');
	if (folder_name)
		mkdir(folder_name, 0777);
	else
		mkdir(dir, 0777);
	ft_strcpy(g_message, "The folder has been successfully created.");
	send(sd, g_message, sizeof(g_message), 0);
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
	else if (ft_strnequ(command, "rm", 2))
		return (do_rm(sd, command));
	else if (ft_strnequ(command, "mkdir", 5))
		return (do_mkdir(sd, command));
	else if (ft_strnequ(command, "quit", 4))
		return (0);
	return (0);
}

int create_socket(char *port)
{
	int sd;
	int enable;
	struct sockaddr_in address;
	
	error_check((sd = socket(AF_INET, SOCK_STREAM, 0)), "socket");
	enable = 1;
	ft_memset(&address, 0, SOCK_SIZE);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(ft_atoi(port));
	error_check(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)), "setsockopt");
	error_check(bind(sd, (struct sockaddr *) &address, sizeof(address)), "bind");
	error_check(listen(sd, 5), "listen");
	printf("listening on port %s\n", port);
	return (sd);
}

void handle_client(int client, struct sockaddr_in client_info)
{
	char command[256];
	
	printf("%s:%d connected\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
	
	ft_memset(command, 0, sizeof(command));
	while (recv(client, command, sizeof(command), 0))
	{
		if (!do_op(client, command))
		{
			printf("%s:%d disconnected\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
			break;
		}
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
        printf("usage: ./s [port]\n");
        exit(-1);
    }

	int sd;

	create_directory();
	sd = create_socket(argv[1]); 
	serve_clients(sd);
}
