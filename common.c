/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 22:53:20 by tcho             ###   ########.fr       */
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

int error_check(int return_value, char *subject)
{
	if (return_value == -1)
	{
		perror(subject);
		exit(EXIT_FAILURE);
	}
	return (return_value);
}

int display(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int send_file_contents(int sd, int fd)
{
    int file_size;
    int nbytes;
    char *file_ptr;
	struct stat fd_info;

    fstat(fd, &fd_info);
	if (!S_ISREG(fd_info.st_mode))
	{
		file_size = -1;
		send(sd, &file_size, sizeof(file_size), 0);
		return (display("Not a regular file.", 1));
	}
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
	return (1);
}
