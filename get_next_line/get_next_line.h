/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 20:06:11 by tcho              #+#    #+#             */
/*   Updated: 2018/11/05 05:37:26 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 1

int	check_new_line(char *buffer, char **container, int fd, char **line);
int	find_next_line(char **container, int fd, char **line);
int	get_next_line(const int fd, char **line);

#endif
