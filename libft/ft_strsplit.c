/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 22:49:00 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 23:19:58 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "libft.h"

static char	*ft_snd(const char *s1, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!(str = (char *)malloc(sizeof(char) * (n + 1))))
		return (NULL);
	while (i < n)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static int	ft_wc(char const *str, char del)
{
	int i;
	int count;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != del)
		{
			count++;
			while (str[i] && (str[i] != del))
				i++;
		}
		else
			i++;
	}
	return (count);
}

static int	ft_wl(char const *str, char del, int i)
{
	int j;

	j = i;
	while (str[j] && (str[j] != del))
		j++;
	return (j - i);
}

char		**ft_strsplit(char const *s, char c)
{
	int		i;
	int		index;
	int		length;
	char	**result;

	i = 0;
	index = 0;
	if (!s || !(result = (char **)malloc(sizeof(char *) *
					(ft_wc(s, c) + 1))))
		return (NULL);
	while (s[i])
	{
		if (s[i] != c)
		{
			length = ft_wl(s, c, i);
			if (!(result[index] = ft_snd(s + i, length)))
				return (NULL);
			index++;
			i += length;
		}
		else
			i++;
	}
	result[index] = NULL;
	return (result);
}
