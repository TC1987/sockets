/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 22:36:18 by tcho             ###   ########.fr       */
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
