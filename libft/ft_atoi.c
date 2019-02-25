/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/22 14:34:20 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 23:24:55 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int			ft_atoi(const char *str)
{
	unsigned long long	result;
	unsigned int		i;
	int					sign;

	result = 0;
	i = 0;
	sign = 1;
	while (ft_is(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = (result * 10) + (str[i] - '0');
		if (result > 9223372036854775807)
			return ((sign == 1) ? -1 : 0);
		i++;
	}
	return (result * sign);
}
