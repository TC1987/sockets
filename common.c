/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 10:12:30 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
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

int count_spaces(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == ' ')
			i++;
		str++;
	}
	return (i);
}

int ft_lstlen(char **list)
{
	int i;

	i = 0;
	if (!list || !(*list))
		return (0);
	while (*list)
	{
		i++;
		list++;
	}
	return (i);
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
