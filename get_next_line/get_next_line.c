/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:31:19 by tcho              #+#    #+#             */
/*   Updated: 2018/11/05 05:19:06 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include "libft.h"
#include "get_next_line.h"

int	check_new_line(char *buffer, char **container, int fd, char **line)
{
	int		index;
	char	*temp;

	if ((index = ft_strfind(buffer, '\n')) >= 0)
	{
		if (!container[fd])
			container[fd] = ft_strnew(1);
		temp = ft_strjoin(container[fd], buffer);
		free(container[fd]);
		container[fd] = temp;
		return (find_next_line(container, fd, line));
	}
	else
	{
		if (container[fd])
		{
			temp = ft_strjoin(container[fd], buffer);
			free(container[fd]);
			container[fd] = temp;
		}
		else
			container[fd] = ft_strdup(buffer);
	}
	return (0);
}

int	find_next_line(char **container, int fd, char **line)
{
	int		index;
	char	*temp;

	index = ft_strfind(container[fd], '\n');
	if (index == -1)
	{
		*line = ft_strdup(container[fd]);
		free(container[fd]);
		container[fd] = NULL;
	}
	else
	{
		*line = ft_strsub(container[fd], 0, index);
		temp = ft_strdup(container[fd] + index + 1);
		free(container[fd]);
		container[fd] = temp;
	}
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	static char	*container[255];
	char		buffer[BUFF_SIZE + 1];
	ssize_t		bytes;

	if (fd < 0 || !line)
		return (-1);
	*line = NULL;
	while ((bytes = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		buffer[bytes] = '\0';
		if (check_new_line(buffer, container, fd, line))
			return (1);
	}
	if (bytes < 0)
		return (-1);
	if (container[fd] && container[fd][0] != '\0')
		return (find_next_line(container, fd, line));
	return (0);
}
