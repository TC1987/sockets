/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 00:19:57 by tcho              #+#    #+#             */
/*   Updated: 2018/11/09 23:23:34 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*node;
	size_t	size_copy;
	void	*content_copy;

	if (!(node = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (content)
	{
		size_copy = content_size;
		content_copy = ft_memalloc(size_copy);
		ft_memcpy(content_copy, content, size_copy);
		node->content = content_copy;
		node->content_size = size_copy;
	}
	else
	{
		node->content = NULL;
		node->content_size = 0;
	}
	node->next = NULL;
	return (node);
}
