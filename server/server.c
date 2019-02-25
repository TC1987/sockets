/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 07:34:15 by tcho              #+#    #+#             */
/*   Updated: 2019/02/25 03:26:23 by tcho             ###   ########.fr       */
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
#include "libft.h"

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

int do_op(char *command)
{
	if (!ft_strncmp(command, "ls", 2))
	{
		execl("/bin/ls", "ls");
	}
	if (!ft_strncmp(command, "cd", 2))
	{
	}
	if (!ft_strncmp(command, "pwd", 3))
	{
	}
	if (!ft_strncmp(command, "get", 3))
	{
		// do_get(command);
	}
	if (!ft_strncmp(command, "put", 3))
	{
	}
	if (!ft_strncmp(command, "quit", 4))
	{
		return (0);
	}
	return (1);
}



int main(int argc, char *argv[])
{
	char buffer[256];
	char path[256];
	int server_socket;
	int client_socket;
	int port;
	struct sockaddr_in address;

	getcwd(path, sizeof(path));
	ft_bzero(buffer, sizeof(buffer));

	if (argc < 2)
		return (error("Usage: ./server port", -1));


	port = ft_atoi(argv[1]);
	ft_strcat(path, "/root");
	mkdir(path, 0700);
	chdir(path);


	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}

	// define the server address
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to a specified IP and port
	if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
	{
		perror("bind");
		exit(-1);
	}

	if (listen(server_socket, 3) == -1)
	{
		perror("listen");
		exit(-1);
	}

	printf("Connected to %d\n", port);

	// accept() returns the client socket that we're going to write to
	// need to pass a struct into accept() if you want to get information about the connected client
	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == -1)
	{
		perror("client socket");
		exit(-1);
	}

	while (ft_strcmp(buffer, "quit") != 0)
	{
		recv(client_socket, &buffer, sizeof(buffer), 0);
		printf("buffer: %s\n", buffer);
		// do_op(buffer);
		send(client_socket, buffer, sizeof(buffer), 0);
		ft_bzero(buffer, sizeof(buffer));
	}
		
	close(server_socket);
}
