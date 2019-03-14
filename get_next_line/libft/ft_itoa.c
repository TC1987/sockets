/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 19:05:19 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 01:01:09 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_itoa(int n)
{
	int		negative;
	int		i;
	int		temp;
	char	*result;

	negative = n < 0 ? 1 : 0;
	i = negative;
	temp = n;
	if (n == 0)
		i = 1;
	while (temp)
	{
		temp /= 10;
		i++;
	}
	if (!(result = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	result[i] = '\0';
	result[--i] = n < 0 ? -(n % 10) + '0' : (n % 10) + '0';
	while (n /= 10)
		result[--i] = n < 0 ? -(n % 10) + '0' : (n % 10) + '0';
	if (negative)
		result[--i] = '-';
	return (result);
}
