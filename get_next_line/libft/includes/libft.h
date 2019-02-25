/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 14:48:40 by tcho              #+#    #+#             */
/*   Updated: 2018/11/04 17:36:36 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>

char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strsub(char const *s, unsigned int start, size_t len);
char	*ft_strnew(size_t size);
void	*ft_memset(void *b, int c, size_t len);
int		ft_strfind(char *str, char c);
size_t	ft_strlen(const char *s);

#endif
