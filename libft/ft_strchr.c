/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 01:20:05 by tcho              #+#    #+#             */
/*   Updated: 2018/11/05 20:30:09 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	character;

	i = 0;
	character = c;
	while (s[i])
	{
		if (s[i] == character)
			return ((char *)(&s[i]));
		i++;
	}
	if (s[i] == character)
		return ((char *)(&s[i]));
	return (NULL);
}
