/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 20:11:19 by tcho              #+#    #+#             */
/*   Updated: 2018/10/21 20:16:17 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *b, int c, size_t len)
{
	char			*str;
	unsigned char	char_to_write;
	size_t			i;

	str = (char *)b;
	char_to_write = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		str[i] = char_to_write;
		i++;
	}
	return (b);
}
