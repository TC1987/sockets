/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 01:32:17 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 00:27:58 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	if (ft_strlen(needle) == 0 || (ft_strcmp(needle, "") == 0))
		return ((char *)haystack);
	while ((i < len) && haystack[i])
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (((i + j) < len) && haystack[i + j] == needle[j])
			{
				j++;
				if (needle[j] == '\0')
					return (&((char *)haystack)[i]);
			}
		}
		i++;
	}
	return (NULL);
}
