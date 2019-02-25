/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 23:43:13 by tcho              #+#    #+#             */
/*   Updated: 2019/02/24 07:55:31 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "libft.h"
#include "get_next_line.h"

/*
 * ls
 * cd
 * get __file__
 * put __file__
 * pwd
 * quit
 * 
 * Specific prompt for the client.
 * Unable to access parent directory of server's executable directory (unless a parameter specified to the server indicates another start directory.
 * Display SUCCESS or ERROR messages + explantation after each request.
 *
 * Server is essentially only listening for commands and responding to whatever commands the client sends to it.
 */

int error(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int validate_command(char *command)
{
	if (!ft_strncmp(command, "ls", 2) ||
		!ft_strncmp(command, "cd", 2) ||
		!ft_strncmp(command, "pwd", 3) ||	
		!ft_strncmp(command, "get", 3) ||
		!ft_strncmp(command, "put", 3) ||
		!ft_strncmp(command, "quit", 4))
		return (1);
	return (0);
}

void display_menu(char *ip_address, int port)
{
	printf("*************************************\n");
	printf("*                                   *\n");
	printf("*   Connected to %s:%d   *\n", ip_address, port);
	printf("*                                   *\n");
	printf("*************************************\n");
}

int main(int argc, char *argv[])
{
	int network_socket;
	char *ip_address;
	int port;
	struct sockaddr_in address;
	char buffer[256];

	ft_bzero(buffer, sizeof(buffer));

	if (argc < 3)
		return (error("Usage: ./client server port", -1));

	ip_address = argv[1];
	port = ft_atoi(argv[2]);

	// 0 is where you would specify the protocol which would be used if this was a raw socket. 0 defaults to TCP in our case.
	if ((network_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Could not create socket.\n");
		exit(-1);
	}

	// `address` specifies where we want to connect to i.e the ip address and port number
	address.sin_family = AF_INET; // Same as the socket type.
	address.sin_port = htons(port);
	// address.sin_addr is a struct itself, so referencing a property within it.
	address.sin_addr.s_addr = inet_addr(ip_address); // Same as connecting to 0.0.0.0 which is any IP address on the computer.

	// Connect to server.
	if (connect(network_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
	{
		printf("Could not connect to server.\n");
		exit(-1);
	}

	display_menu(ip_address, port);
	
	while (1)
	{
		printf("> ");
		fflush(stdout);
		char *command;
		get_next_line(0, &command);

		if (!validate_command(command))
			return (error("Not a valid command.", -1));
		if (ft_strcmp(command, "quit") == 0)
			break;
		ft_strcpy(buffer, command);
		send(network_socket, buffer, sizeof(buffer), 0);
		bzero(buffer, sizeof(buffer));
		recv(network_socket, &buffer, sizeof(buffer), 0);
		printf("The server sent the data: %s\n", buffer);
	}

	close(network_socket);
}
