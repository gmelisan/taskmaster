/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_str_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 21:21:43 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/26 14:04:19 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void		free_all_str_list(t_str_list *list)
{
	t_str_list 	*tmp;

	if (!list)
		return ;
	while (list->next)
	{
		tmp = list;
		list = list->next;
		free_one_str_list(tmp);
	}
	free_one_str_list(list);
}
