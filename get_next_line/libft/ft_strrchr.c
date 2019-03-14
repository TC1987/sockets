/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 01:22:53 by tcho              #+#    #+#             */
/*   Updated: 2018/11/05 16:49:01 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last;
	char	character;

	i = 0;
	last = NULL;
	character = c;
	while (s[i])
	{
		if (s[i] == character)
			last = ((char *)&s[i]);
		i++;
	}
	if (s[i] == character)
		last = ((char *)&s[i]);
	return (last);
}
