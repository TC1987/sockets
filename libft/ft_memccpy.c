/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 00:30:04 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 00:23:51 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memccpy(void *restrict dst, const void *restrict src,
		int c, size_t n)
{
	size_t			i;
	unsigned char	*source;
	unsigned char	*destination;
	unsigned char	to_find;

	i = 0;
	source = (unsigned char *)src;
	destination = (unsigned char *)dst;
	to_find = (unsigned char)c;
	while (i < n)
	{
		destination[i] = source[i];
		if (source[i] == to_find)
			return (dst + i + 1);
		i++;
	}
	return (NULL);
}
