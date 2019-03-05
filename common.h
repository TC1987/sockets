/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:37:59 by tcho              #+#    #+#             */
/*   Updated: 2019/03/04 22:51:37 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

#define SOCK_SIZE sizeof(struct sockaddr_in)

int error_check(int, char *);
int display(char *, int);
int send_file_contents(int sd, int fd);

#endif
