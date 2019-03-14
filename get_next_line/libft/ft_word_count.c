/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 04:08:25 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 10:34:15 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_word_count(char const *str, char del)
{
	int count;

	count = 0;
	while (*str)
	{
		if (*str != del)
		{
			count++;
			while (*str && (*str != del))
				str++;
		}
		else
			str++;
	}
	return (count);
}
