/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:27:28 by tcho              #+#    #+#             */
/*   Updated: 2019/02/28 03:35:07 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
