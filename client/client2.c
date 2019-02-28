#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"
#include "common.h"

char g_pwd[256];

int error(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int check_command(char *command)
{
	if (ft_strnequ(command, "ls", 2) ||
		ft_strnequ(command, "cd", 2) ||
		ft_strnequ(command, "pwd", 3) ||
		ft_strnequ(command, "get", 3) ||
		ft_strnequ(command, "put", 3) ||
		ft_strnequ(command, "quit", 4))
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

	send(sd, command, ft_strlen(command), 0);
	args = ft_strsplit(command, ' ');
	if ((file_name = ft_strrchr(command, '/')))
		file_name++;
	else
		file_name = args[1];
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

int get_pwd(int sd, char *command)
{
	if (ft_lstlen(ft_strsplit(command, ' ')) != 1)
		printf("pwd only takes 1 argument. You should know better.\n");
	else
	{
		ft_memset(g_pwd, 0, sizeof(g_pwd));
		send(sd, command, ft_strlen(command), 0);
		recv(sd, g_pwd, sizeof(g_pwd), 0);
		printf("%s\n", g_pwd);
	}
	return (1);
}

int do_cd(int sd, char *command)
{
	char **args;

	args = ft_strsplit(command, ' ');
	if (ft_lstlen(args) != 2)
		printf("Usage: cd path\n");
	else
	{
		send(sd, command, ft_strlen(command), 0);
		// get_pwd(sd, command);
	}
	return (1);
}

int do_op(int socket, char *command)
{
	if (ft_strnequ(command, "ls", 2))
	{
		return (1);
	}
	if (ft_strnequ(command, "cd", 2))
	{
		return (do_cd(socket, command));
	}
	if (ft_strnequ(command, "pwd", 3))
	{
		return (get_pwd(socket, command));
	}
	if (ft_strnequ(command, "get", 3))
	{
		return (get_file(socket, command));
	}
	if (ft_strnequ(command, "put", 3))
	{
		return (1);
	}
	if (ft_strnequ(command, "quit", 4))
	{
		send(socket, command, ft_strlen(command), 0);
		return (0);
	}
	return (0);
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
	// char path[256];

	while (keep_alive)
	{
		/*
		recv(sd, path, sizeof(path), 0);
		printf("%s $ ", path);
		fflush(stdout);
		*/
		ft_putstr(">> ");
		get_next_line(0, &command);

		if (check_command(command) == 0)
		{
			close(sd);
			return (error("Not a valid command", -1));
		}
		keep_alive = do_op(sd, command);
		// ft_memset(path, 0, sizeof(path));
	}
    close(sd);
}
