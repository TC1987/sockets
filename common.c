/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/03/14 00:27:26 by tcho             ###   ########.fr       */
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

void	free_list(char **list)
{
	char **head;

	if (!list || !(*list))
		return;
	head = list;
	while (*head)
	{
		free(*head);
		head++;
	}
	free(list);
}

int		error_check(int return_value, char *subject)
{
	if (return_value == -1)
	{
		perror(subject);
		exit(EXIT_FAILURE);
	}
	return (return_value);
}

int		error_check_return(int return_value, char *subject)
{
	if (return_value == -1)
	{
		perror(subject);
		return (0);
	}
	return (1);
}

int		display(char *message, int code)
{
	printf("%s\n", message);
	return (code);
}

int		check_file(int sd, int fd, struct stat fd_info)
{
	int file_size;

	if (fd == -1 || !S_ISREG(fd_info.st_mode))
	{
		file_size = -1;
		send(sd, &file_size, sizeof(file_size), 0);
		return (0);
	}
	return (1);
}

int		send_file_contents(int sd, int fd)
{
	int			file_size;
	int			nbytes;
	char		*file_ptr;
	char		*head;
	struct stat	fd_info;

	fstat(fd, &fd_info);
	if (!check_file(sd, fd, fd_info))
		return (display("Uploads are restricted to regular files.", 0));
	file_size = fd_info.st_size;
	send(sd, &file_size, sizeof(int), 0);
	file_ptr = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	head = file_ptr;
	while (file_size > 0 && \
			(nbytes = send(sd, file_ptr, fd_info.st_size, 0)) != -1)
	{
		printf("%d / %lld bytes sent\n", nbytes, fd_info.st_size);
		file_ptr += nbytes;
		file_size -= nbytes;
	}
	munmap(head, fd_info.st_size);
	close(fd);
	return (1);
}
