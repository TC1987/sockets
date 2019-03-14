/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcho <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 01:32:29 by tcho              #+#    #+#             */
/*   Updated: 2018/11/08 22:05:05 by tcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *new_list;
	t_list *result;
	t_list *current;

	if (!lst || !f)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		if (!(result = f(lst)))
			return (NULL);
		if (!new_list)
			new_list = result;
		else
		{
			current = new_list;
			while (current->next)
				current = current->next;
			current->next = result;
		}
		result->next = NULL;
		lst = lst->next;
	}
	return (new_list);
}
