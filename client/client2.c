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

// 1. rm
// 2. mkdir
// 3. get completion
// 4. put completion
// 5. lls, lcd, lpwd

char	g_message[4096];

int error(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int check_command(char *command)
{
	if (ft_strnequ(command, "ls", 2) || ft_strnequ(command, "cd", 2) ||
		ft_strnequ(command, "pwd", 3) || ft_strnequ(command, "get", 3) ||
		ft_strnequ(command, "put", 3) || ft_strnequ(command, "quit", 4) ||
		ft_strnequ(command, "lrm", 2) || ft_strnequ(command, "lcd", 3) ||
		ft_strnequ(command, "lls", 3) || ft_strnequ(command, "lpwd", 4) ||
		ft_strnequ(command, "lmkdir", 6))
		return (1);
	return (0);
}

int put_file(int sd, char *command)
{
    int fd;
	char *file;

	if (ft_word_count(command, ' ') != 2)
		return (display("usage: put [file_name]", 1));
	file = ft_strrchr(command, ' ') + 1;
    if ((fd = open(file, O_RDONLY)) == -1)
		return (display("File does not exist or you do not have permissions.", 1));
   	send(sd, command, ft_strlen(command), 0);
	if (send_file_contents(sd, fd))
		printf("%s has been successfully uploaded.\n", file);
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
	close(fd);
}

void write_file_contents(int sd, int file_size, char *file_name)
{
    int fd;
    int nbytes;
	int remaining;
    char *buffer;

    buffer = malloc(sizeof(char) * file_size);
	remaining = file_size;
    error_check((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777)), "open");
    while (remaining > 0 && (nbytes = recv(sd, buffer, file_size, 0)) > 0)
    {
        write(fd, buffer, nbytes);
        remaining -= nbytes;
    }
    close(fd);
}

int write_file(int sd, char *file_name)
{
	int file_size;

    recv(sd, &file_size, sizeof(file_size), 0);
	if (file_size == -1)
		return (display("You do not have permissions to this file or it does not exist.", 0));
	write_file_contents(sd, file_size, file_name);
	return (1);
}

int get_file(int sd, char *command)
{
	char *file_name;

	if (ft_word_count(command, ' ') != 2)
		return (display("usage: get [file_name]", 1));
	send(sd, command, ft_strlen(command), 0);
	file_name = ft_strrchr(command, '/') ? ft_strrchr(command, '/') + 1 : ft_strrchr(command, ' ') + 1;
	if (write_file(sd, file_name))
		printf("%s has successfully downloaded.\n", file_name);
	return (1);
}

int do_ls(int sd, char *command)
{
	send(sd, command, ft_strlen(command), 0);
	write_file(sd, ".ls");
	read_file(".ls");
	unlink(".ls");
	return (1);
}

int do_cd(int sd, char *command)
{
	if (ft_word_count(command, ' ') != 2)
		printf("usage: cd [path]\n");
	else
	{
		send(sd, command, ft_strlen(command), 0);
		recv(sd, g_message, sizeof(g_message), 0);
		printf("%s\n", g_message);
	}
	return (1);
}

int do_quit(int sd, char *command)
{
	send(sd, command, ft_strlen(command), 0);
	return (0);
}

int get_pwd(int sd, char *command)
{
	char buffer[SIZE];

	if (ft_word_count(command, ' ') != 1)
		printf("usage: pwd\n");
	else
	{
		send(sd, command, ft_strlen(command), 0);
		recv(sd, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
	}
	return (1);
}

int do_lrm(char *command)
{
	char *file;

	if (ft_word_count(command, ' ') != 2)
		printf("usage: rm [file_name]\n");
	else
	{
		file = ft_strrchr(command, ' ') + 1;
		unlink(file);
	}
	return (1);
}

int do_lcd(char *command)
{
	char *path;

	path = ft_strrchr(command, ' ') + 1;
	chdir(path);
	return (1);
}

int do_lls(char *command)
{
	pid_t pid;
	char **args;

	args = ft_strsplit(command, ' ');
	pid = fork();
	if (pid == 0)
		execv("/bin/ls", args);
	else
	{
		wait4(pid, NULL, 0, NULL);
		// free_list(args);
	}
	return (1);
}

int do_lpwd(char *command)
{
	char *pwd;

	if (ft_word_count(command, ' ') != 1)
		printf("usage: lpwd\n");
	else
	{
		pwd = getcwd(NULL, 0);
		printf("%s\n", pwd);
		free(pwd);
	}
	return (1);
}

int do_lmkdir(char *command)
{
	char *directory;

	directory = ft_strrchr(command, ' ') + 1;
	mkdir(directory, 0777);
	return (1);
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
	if (ft_strnequ(command, "lrm", 2))
		return (do_lrm(command));
	if (ft_strnequ(command, "lcd", 3))
		return (do_lcd(command));
	if (ft_strnequ(command, "lls", 3))
		return (do_lls(command));
	if (ft_strnequ(command, "lpwd", 4))
		return (do_lpwd(command));
	if (ft_strnequ(command, "lmkdir", 6))
		return (do_lmkdir(command));
	return (0);
}

int create_and_connect(char *ip_address, char *port)
{
	int sd;
	struct sockaddr_in address;

	error_check((sd = socket(AF_INET, SOCK_STREAM, 0)), "socket");
	ft_memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ft_strequ(ip_address, "localhost") ? inet_addr("127.0.0.1") : inet_addr(ip_address);
	address.sin_port = htons(ft_atoi(port));
	error_check(connect(sd, (struct sockaddr *) &address, sizeof(address)), "connect");
	return (sd);
}

void handle_requests(int sd)
{
	int keep_alive;
	char *command;

	keep_alive = 1;
	while (keep_alive)
	{
		ft_putstr(">> ");
		get_next_line(0, &command);
		if (ft_strequ(command, ""))
			continue;
		else if (check_command(command) == 0)
			printf("Not a valid command.\n");
		else
			keep_alive = do_op(sd, command);
		free(command);
	}
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./c [ip] [port]\n");
        exit(-1);
    }

	int sd;

	sd = create_and_connect(argv[1], argv[2]);
    printf("connection to %s:%s successful.\n", argv[1], argv[2]);
	handle_requests(sd);
    close(sd);
}
