/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 23:43:13 by tcho              #+#    #+#             */
/*   Updated: 2019/02/27 04:36:28 by tcho             ###   ########.fr       */
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
#include <fcntl.h>
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
	if (ft_strnequ(command, "ls", 2) ||
			ft_strnequ(command, "cd", 2) ||
			ft_strnequ(command, "pwd", 3) ||	
			ft_strnequ(command, "get", 3) ||
			ft_strnequ(command, "put", 3) ||
			ft_strnequ(command, "quit", 4))
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

int do_get(int socket, char *command)
{
	int	file_size;
	char buffer[4096];
	int fd;
	char client_path[256];
	char *full_path;
	char **args = ft_strsplit(command, ' ');
	ssize_t bytes_read;

	ft_memset(buffer, 0, sizeof(buffer));

	// Get current path for client.
	
	getcwd(client_path, sizeof(client_path));

	// Send command to server.
	
	send(socket, command, ft_strlen(command), 0);

	// Create a file with the same name as the one being retrieved from the server. File needs to be O_WRONLY, O_APPEND, O_CREAT.
	
	full_path = get_full_path(client_path, args[1]);

	printf("full_path: %s\n", full_path);
	fd = open(full_path, O_WRONLY | O_APPEND | O_CREAT, 0777);

	// Get file size from server.
	
	recv(socket, &file_size, sizeof(file_size), 0);
	
	printf("size: %d\n", file_size);

	/* ---------------------------------------------------- */

	// While file_size is greater than 0 and write is successful, read more data and write.
	
	while (file_size > 0 && (bytes_read = recv(socket, &buffer, sizeof(buffer), 0)) > 0)
	{
		printf("READING FILE\n");
		write(fd, &buffer, sizeof(bytes_read));
		file_size -= bytes_read;
	}

	// Once finished, close the file.
	
	close(fd);
	return (1);
}

int do_op(int socket, char *command)
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
    	return (do_get(socket, command));
	}
	if (ft_strnequ(command, "put", 3))
    {
    }
    if (ft_strnequ(command, "quit", 4))
    {
    	// return (do_close(socket));
    }
    return (1);
}

int main(int argc, char *argv[])
{
	int connection_socket;
	char buffer[256];

	if (argc < 3)
		return (error("Usage: ./client server port", -1));

	connection_socket = create_socket();
	connect_server(connection_socket, argv[1], ft_atoi(argv[2]));

	ft_memset(buffer, 0, sizeof(buffer));

	display_menu(argv[1], ft_atoi(argv[2]));

	// Issue with this while loop because and recv, nothing stops the while loop from constantly going.
	
	int alive = 1;

	// Get server's path from server.
	char *path;
	ft_memset(buffer, 0, sizeof(buffer));
	recv(connection_socket, &buffer, sizeof(buffer), 0);
	path = ft_strdup(buffer);

	while (alive)
	{
		printf("> ");
		fflush(stdout);

		char *command;
		get_next_line(0, &command);

		if (validate_command(command) == 0)
		{
			close(connection_socket);
			return (error("Not a valid command.", -1));
		}

		// Need a do_op like on the server side.
		alive = do_op(connection_socket, command);
		
		/*
		send(connection_socket, command, ft_strlen(command), 0);

		ft_memset(buffer, 0, sizeof(buffer));

		recv(connection_socket, &buffer, sizeof(buffer), 0);
		*/

		printf("The server sent: %s\n", buffer);

		if (ft_strequ(buffer, "Disconnected from server."))
			break;
	}

	close(connection_socket);
}
