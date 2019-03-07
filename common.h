/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 03:37:59 by tcho              #+#    #+#             */
/*   Updated: 2019/03/06 20:25:01 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

#define SOCK_SIZE sizeof(struct sockaddr_in)

void free_list(char **);
int error_check(int, char *);
int error_check_return(int, char *);
int display(char *, int);
int send_file_contents(int sd, int fd);

#endif
