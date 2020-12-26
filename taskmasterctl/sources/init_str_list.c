/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_str_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 15:24:52 by ggwin-go          #+#    #+#             */
/*   Updated: 2020/10/18 15:50:29 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

t_str_list  *init_str_list(t_str_list *prev)
{
    t_str_list	*lex;

    lex = (t_str_list*)ft_xmalloc(sizeof(t_str_list));
    lex->word = NULL;
    lex->next = NULL;
    lex->prev = prev;
    return (lex);
}
