/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/03/06 21:42:53 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include "libft.h"
#include "common.h"

void free_list(char **list)
{
	if (!list || !(*list))
		return;

	char **head;

	head = list;
	while (*head)
	{
		free(*head);
		head++;
	}
	free(list);
}

int error_check(int return_value, char *subject)
{
	if (return_value == -1)
	{
		perror(subject);
		exit(EXIT_FAILURE);
	}
	return (return_value);
}

int error_check_return(int return_value, char *subject)
{
	if (return_value == -1)
	{
		perror(subject);
		return (0);
	}
	return (1);
}

int display(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

// Need to find all instances of send_file_contents and make sure to display a message if file is not a regular file.

int send_file_contents(int sd, int fd)
{
    int file_size;
    int nbytes;
    char *file_ptr;
	struct stat fd_info;

    fstat(fd, &fd_info);
	if (fd == -1 || !S_ISREG(fd_info.st_mode))
	{
		file_size = -1;
		send(sd, &file_size, sizeof(file_size), 0);
		return (display("Must be a valid file.", 0));
	}
    file_size = fd_info.st_size;
    send(sd, &file_size, sizeof(file_size), 0);
    file_ptr = mmap(NULL, fd_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    while (file_size > 0 && ((nbytes = send(sd, file_ptr, fd_info.st_size, 0)) != -1))
    {
		printf("%d / %lld bytes sent\n", nbytes, fd_info.st_size);
        file_ptr += nbytes;
        file_size -= nbytes;
    }
    munmap(file_ptr, fd_info.st_size);
    close(fd);
	return (1);
}
