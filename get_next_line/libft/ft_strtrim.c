/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 03:16:24 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 23:24:37 by tcho             ###   ########.fr       */
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

static int	ft_is(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

char		*ft_strtrim(char const *s)
{
	int		i;
	int		j;
	int		k;
	char	*result;

	if (!s)
		return (NULL);
	i = 0;
	j = ft_strlen(s);
	k = 0;
	while (s[i] && ft_is(s[i]))
		i++;
	while (j > 0 && ft_is(s[j - 1]))
		j--;
	if (j < i)
		result = ft_strdup("");
	else
		result = ft_snd(s + i, j - i);
	return (result);
}
