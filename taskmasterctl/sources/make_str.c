/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 15:24:52 by ggwin-go          #+#    #+#             */
/*   Updated: 2020/10/18 15:50:29 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*make_str(t_str_list *lex)
{
    char	*str;

    str = ft_xstrdup(lex->word);
    while ((lex = lex->next))
    {
        str = ft_strrejoin(str, "\n", 1);
        str = ft_strrejoin(str, lex->word, 1);
    }
    return str;
}
