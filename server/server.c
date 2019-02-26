/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 07:34:15 by tcho              #+#    #+#             */
/*   Updated: 2019/02/26 07:57:18 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "libft.h"

#define EXIT 0

int error(char *message, int code)
{
    printf("%s\n", message);
    return (code);
}

/*
   int do_get(char *command)
   {
   char *file;
   int fd;

   file = ft_strchr(command, ' ');
   file++;
   if ((fd = open(file, O_RDONLY )))
   {

   }
   }
   */

int do_close(int socket)
{
    char message[] = "Disconnected from server.";
    
    printf("Client has disconnected.\n");
    send(socket, message, sizeof(message), 0);
    close(socket);
	return (0);
}

int ft_lstlen(char **list)
{
	int len;

	if (!list || !(*list))
		return (0);
	len = 0;
	while (*list)
	{
		len++;
		list++;
	}
	return (len);
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

int do_get(int socket, char *command, char *path)
{
	char		**args;
	int			len;
	int			fd;
	struct stat	file_info;	
	char		*full_path;
	char		buffer[256];
	int			file_size;
	int			bytes_read;

	args = ft_strsplit(command, ' ');
	len = ft_lstlen(args);
	if (len != 2)
		return (0);

	full_path = get_full_path(path, args[1]);
	printf("%s\n", full_path);

	if ((fd = open(full_path, O_RDONLY)) == -1)
	{
		perror("open file");
		return (0);
	}

	fstat(fd, &file_info);
	file_size = file_info.st_size;
	
	// Send file_size to client.
	
	send(socket, &file_size, sizeof(file_size));

	// Send file contents while read returns something greater than 0 and file_size is greater than 0.
	// Make amount of bytes read (i.e 3rd argument in read) be dynamic.

	while ((bytes_read = read(fd, buffer, 256) > 0) && (file_size > 0))
	{
		send(socket, &buffer, sizeof(buffer), 0);
		file_size -= bytes_read;
	}

	close(fd);
	return (0);
}

int do_op(int socket, char *command, char *path)
{
    if (ft_strnequ(command, "ls", 2))
    {
        execl("/bin/ls", "ls");
    }
    if (ft_strnequ(command, "cd", 2))
    {
    }
    if (ft_strnequ(command, "pwd", 3))
    {
    }
    if (ft_strnequ(command, "get", 3))
    {
        return (do_get(socket, command, path));
    }
    if (ft_strnequ(command, "put", 3))
    {
    }
    if (ft_strnequ(command, "quit", 4))
    {
        return (do_close(socket));
    }
    return (1);
}

char *create_directory(void)
{
    char *path;
	int path_size = 256;

    if ((path = malloc(sizeof(char) * path_size)) == NULL)
        return (NULL);
    getcwd(path, path_size);
    ft_strcat(path, "/root");
    mkdir(path, 0700);
    chdir(path);
    return (path);
}

int create_socket(void)
{
    int server_socket;
    int option_value;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    return (server_socket);	
}

void bind_socket(int server_socket, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        perror("bind");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    int		client_socket;
    int		server_socket;
    char	buffer[256];
    char	*path;

    if (argc < 2)
        return (error("Usage: ./server port", -1));

    ft_memset(buffer, 0, sizeof(buffer));

    path = create_directory();
    server_socket = create_socket();	
    bind_socket(server_socket, ft_atoi(argv[1]));
    listen(server_socket, 3);

    printf("waiting for connection...\n");

    struct sockaddr_in client_info;
    socklen_t size = sizeof(client_info);
    pid_t pid;

    while ((client_socket = accept(server_socket, (struct sockaddr *) &client_info, &size)))
    {
        if (client_socket == -1)
        {
            perror("client socket");
            exit(-1);
        }

        printf("client connected: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

        if ((pid = fork()) == -1)
            close(client_socket);
        else if (pid == 0)
        {
            close(server_socket);

			int alive = 1;
            while (alive)
            {
                recv(client_socket, &buffer, sizeof(buffer), 0);
                printf("%s:%d: %s\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port), buffer);
                alive = do_op(client_socket, buffer, path);
                send(client_socket, buffer, sizeof(buffer), 0);
                ft_memset(buffer, 0, sizeof(buffer));
            }
			close(client_socket);
        }
        else
            close(client_socket);
    }	

    close(server_socket);
}
