/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 07:34:15 by tcho              #+#    #+#             */
/*   Updated: 2019/02/25 11:17:07 by tcho             ###   ########.fr       */
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

// USE FT_STRNEQU INSTEAD.
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

char *create_directory(void)
{
	char *path;

	if ((path = malloc(sizeof(char) * 256)) == NULL)
		return (NULL);
	getcwd(path, sizeof(path));
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
			while (1)
			{
				recv(client_socket, &buffer, sizeof(buffer), 0);
				printf("%s:%d: %s\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port), buffer);
				if (ft_strequ(buffer, "quit"))
				{
					printf("DISCONNECTED\n");
					char message[] = "Disconnected from server.";
					send(client_socket, message, sizeof(message), 0);
					break;
				}
				send(client_socket, buffer, sizeof(buffer), 0);
				ft_memset(buffer, 0, sizeof(buffer));
			}
			close(client_socket);
		}
		else
		{
			close(client_socket);
		}
	}	

}
