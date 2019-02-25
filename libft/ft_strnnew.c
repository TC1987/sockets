/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 09:50:49 by tcho              #+#    #+#             */
/*   Updated: 2018/12/08 09:51:22 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char	*ft_strnnew(char *str, size_t n)
{
	char	*res;
	size_t	i;

	res = (char *)malloc(sizeof(char) * n);
	i = 0;
	while (i < n)
	{
		res[i] = str[i];
		i++;
	}
	return (res);
}
