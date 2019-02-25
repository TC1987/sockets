/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 00:34:32 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 00:26:08 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*destination;
	char	*source;

	destination = (char *)dst;
	source = (char *)src;
	if (dst > src)
	{
		while (len)
		{
			*(destination + len - 1) = *(source + len - 1);
			len--;
		}
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}
