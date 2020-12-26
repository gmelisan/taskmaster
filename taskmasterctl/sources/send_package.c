/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_package.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 15:24:52 by ggwin-go          #+#    #+#             */
/*   Updated: 2020/10/18 15:50:29 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int
send_package(t_str_list *lex, int fd)
{
    char    *str;
    char    *buf;
    int     size;

    str = make_str(lex);
    encode_message(str, &buf, &size);
    write(fd, buf, size);
    ft_strdel(&str);
    ft_strdel(&buf);
    return (0);
}
