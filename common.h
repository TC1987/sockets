/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:37:59 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 09:10:02 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>

#define SOCK_SIZE sizeof(struct sockaddr_in)

int error_check(int, char *);
int display(char *, int);
int ft_lstlen(char **);
char *get_full_path(char *, char *);
int count_spaces(char *);

int ft_strlastindex(char *, char);

#endif
