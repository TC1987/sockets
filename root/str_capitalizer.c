/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_capitalizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/19 18:23:44 by tcho              #+#    #+#             */
/*   Updated: 2019/01/20 16:44:36 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int is_character(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int is_upper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

int is_lower(char c)
{
	return (c >= 'a' && c <= 'z');
}

void ft_putchar(char c)
{
	write(1, &c, 1);
}

void make_upper(char *str, int i)
{
	if (is_lower(str[i]))
		str[i] = str[i] - 32;
}

void make_lower(char *str, int i)
{
	if (is_upper(str[i]))
		str[i] = str[i] + 32;
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		int index = 1;

		while (index < argc)
		{
			char *str = argv[index];
			int i = 0;

			if (str[i] && is_character(str[i]))
				print_upper(str[i++]);

			while (str[i])
			{
				if (is_space(str[i - 1]) && is_character(str[i]))
					make_upper(str, i);
				else if (is_character(str[i]))
					make_lower(str, i);
				i++;
			}
			write(1, str, i);
			write(1, "\n", 1);
			index++;
		}
	}
}
