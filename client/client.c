/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 23:43:13 by tcho              #+#    #+#             */
/*   Updated: 2019/02/25 11:30:44 by tcho             ###   ########.fr       */
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
    while (1)
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
        
        send(connection_socket, command, ft_strlen(command), 0);
        
        ft_memset(buffer, 0, sizeof(buffer));
        
        recv(connection_socket, &buffer, sizeof(buffer), 0);
        
        printf("The server sent: %s\n", buffer);

        if (ft_strequ(buffer, "Disconnected from server."))
            break;
    }

    close(connection_socket);
}

char *str = "hello world";

some_function(str, "star wars")

str = "star wars";
