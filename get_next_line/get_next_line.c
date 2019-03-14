/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 00:15:44 by tcho              #+#    #+#             */
/*   Updated: 2019/03/13 17:04:30 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "libft.h"

#define BUFF_SIZE 256

int	ft_findindex(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && (str[i] != c))
		i++;
	return ((str[i]) ? i : -1);
}

int	set_line(char **container, int fd, int bytes, char **line)
{
	int		newline_index;
	char	*tmp;

	if (!&container[fd] || !container[fd])
		return (0);
	if (bytes >= 0)
	{
		if ((newline_index = ft_findindex(container[fd], '\n')) == -1)
		{
			*line = ft_strdup(container[fd]);
			free(container[fd]);
			return (0);
		}
		*line = ft_strndup(container[fd], newline_index);
		tmp = container[fd];
		container[fd] = ft_strdup(container[fd] + newline_index + 1);
		free(tmp);
		return (1);
	}
	return (-1);
}

int	get_next_line(const int fd, char **line)
{
	static char	*container[256];
	char		buffer[BUFF_SIZE + 1];
	ssize_t		bytes;
	char		*tmp;

	if (fd < 0 || !line)
		return (-1);
	while ((bytes = read(fd, buffer, BUFF_SIZE)))
	{
		buffer[bytes] = '\0';
		if (!container[fd])
			container[fd] = ft_strnew(1);
		tmp = ft_strjoin(container[fd], buffer);
		free(container[fd]);
		container[fd] = tmp;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (set_line(container, fd, bytes, line));
}
