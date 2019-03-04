#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"
#include "get_next_line.h"
#include "common.h"

#define SIZE 256

char g_pwd[SIZE];

int error(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int check_command(char *command)
{
	if (ft_strnequ(command, "ls", 2) || ft_strnequ(command, "cd", 2) ||
		ft_strnequ(command, "pwd", 3) || ft_strnequ(command, "get", 3) ||
		ft_strnequ(command, "put", 3) || ft_strnequ(command, "quit", 4))
		return (1);
	return (0);
}

int get_file(int sd, char *command)
{
    int fd;
    int file_size;
    char *buffer;
    int nbytes;
	char *file_name;
	char **args;
	int remaining;

	printf("%s\n", command);
	send(sd, command, ft_strlen(command), 0);
	args = ft_strsplit(command, ' ');
	if ((file_name = ft_strrchr(command, '/')))
		file_name++;
	else
		file_name = args[1];
    recv(sd, &file_size, sizeof(file_size), 0);
    buffer = malloc(sizeof(char) * file_size);
	remaining = file_size;
    fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
    while (remaining > 0 && (nbytes = recv(sd, buffer, file_size, 0)) > 0)
    {
        write(fd, buffer, nbytes);
        remaining -= nbytes;
    }
    close(fd);
	printf("%s has successfully downloaded.\n", file_name);
	return (1);
}

int put_file(int sd, char *command)
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
	{
		printf("Usage: put file_name\n");
		return (1);
	}
	send(sd, command, ft_strlen(command), 0);
	full_path = get_full_path(g_pwd, args[1]);
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
	// free(args);
	return (1);
}

int get_pwd(int sd, char *command)
{
	char buffer[SIZE];

	if (count_spaces(command) != 0)
		printf("pwd only takes 1 argument. You should know better.\n");
	else
	{
		send(sd, command, ft_strlen(command), 0);
		recv(sd, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
	}
	return (1);
}

char *expand(char *prefix, char *suffix)
{
	char *path;
	char *head;
	int path_length;
	int i;

	i = 0;
	path_length = (ft_strequ(suffix, ".")) ? ft_strlen(g_pwd) : ft_strlastindex(g_pwd, '/');
	path = ft_strnew(ft_strlen(prefix) + 1 + path_length + 1 + ft_strlen(suffix) + 1);
	head = path;
	while (*prefix)
		*path++ = *prefix++;
	*path++ = ' ';
	while (i < path_length)
		*path++ = g_pwd[i++];
	*path++ = '/';
	while (*suffix)
		*path++ = *suffix++;
	*path = '\0';
	return (head);
}

int do_cd(int sd, char *command)
{
	printf("%s\n", command);

	char **args;
	char *expanded;

	args = ft_strsplit(command, ' ');
	expanded = command;
	if (ft_lstlen(args) != 2)
		printf("Usage: cd path\n");
	else
	{
		send(sd, command, ft_strlen(command), 0);
		/*
		if ((args[1][0] == '.') || ft_strnequ(args[1], "..", 2))
		{
			location = ft_strrchr(args[1], '.');
			if (ft_strlen(location) == 1)
				expanded = expand(args[0], location + 1);
			else if (ft_strlen(location) >= 2)
				expanded = expand(args[0], location + 2);
		}
		send(sd, expanded, ft_strlen(expanded), 0);
		*/
		// get_pwd(sd, command); // Issue because get_pwd has send() and send_pwd on the server end just has send().
	}
	// free(args);
	// free(expanded);
	return (1);
}

void read_file(char *file)
{
	int fd;
	char buffer[256];
	int bytes;

	fd = open(file, O_RDONLY);
	while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes] = '\0';
		printf("%s", buffer);
	}
}

int do_ls(int sd, char *command)
{
	send(sd, command, ft_strlen(command), 0);
	recv(sd, NULL, 0, 0);
	get_file(sd, "get ls");
	read_file("ls");
	unlink("ls");
	return (1);
}

int do_quit(int sd, char *command)
{
	send(sd, command, ft_strlen(command), 0);
	return (0);
}

int do_op(int socket, char *command)
{
	if (ft_strnequ(command, "ls", 2))
		return (do_ls(socket, command));
	if (ft_strnequ(command, "cd", 2))
		return (do_cd(socket, command));
	if (ft_strnequ(command, "pwd", 3))
		return (get_pwd(socket, command));
	if (ft_strnequ(command, "get", 3))
		return (get_file(socket, command));
	if (ft_strnequ(command, "put", 3))
		return (put_file(socket, command));
	if (ft_strnequ(command, "quit", 4))
		return (do_quit(socket, command));
	return (0);
}

int create_socket(void)
{
	int server_socket;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}
	return (server_socket);
}

void connect_server(int connection_socket, char *ip_address, int port)
{
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip_address);;
	if (connect(connection_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
	{
		printf("Could not connect to server.\n");
		exit(-1);
	}
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

    printf("Connection to %s:%s successful.\n", argv[1], argv[2]);
	printf("Available functions: get pwd quit\n");

	int keep_alive = 1;
	char *command;
	
    // ft_memset(g_pwd, 0, sizeof(g_pwd));
	// recv(sd, g_pwd, sizeof(g_pwd), 0);

	while (keep_alive)
	{
		/*
		printf("%s $ ", path);
		fflush(stdout);
		*/
		ft_putstr(">> ");
		get_next_line(0, &command);
		
		if (ft_strequ(command, ""))
			continue;
		else if (check_command(command) == 0)
			printf("Not a valid command\n");
		else
			keep_alive = do_op(sd, command);
		free(command);
		// ft_memset(path, 0, sizeof(path));
	}
    close(sd);
}
