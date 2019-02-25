/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 01:12:19 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 22:55:53 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t dsize;
	size_t ssize;
	size_t i;
	size_t j;

	dsize = ft_strlen(dst);
	ssize = ft_strlen(src);
	i = 0;
	j = dsize;
	if (dstsize < dsize + 1)
		return (ssize + dstsize);
	while (i < dstsize - dsize - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	if (!(dstsize == 0 || (dsize > dstsize)))
		dst[j] = '\0';
	return (dsize + ssize);
}
