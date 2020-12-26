/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_message.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 22:41:23 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/27 15:21:01 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

-> "start nginx"
	' ' -> '\n', '\0' -> '\n'
	len("start nginx") = 11 = 0xB
    + 1 last \n = 0xC

<- [0][0][0][0xC][s][t][a][r][t][\n][n][g][i][n][x][\n]

*/
#include <string.h>
#include "ft_socket.h"

int
decode_message(const char *buf, char ***p_msgv, int *p_msgc)
{
    char	**msgv;
    int		len;
    int		msgc = 0;
    int		i;
    int		j;
    int		word_size;

    if (!buf || !p_msgv || !p_msgc)
        return -1;
    memcpy(&len, buf, sizeof(int));
    for (i = sizeof(int); i < len + (int)sizeof(int); ++i)
	{
        if (buf[i] == '\n')
            ++msgc;
    }
    msgv = (char **)malloc(sizeof(*msgv) * (msgc + 1));
    i = sizeof(int);
    for (j = 0; j < msgc; ++j)
	{
        word_size = i;
        while (buf[i] != '\n')
            ++i;
        word_size = i - word_size;
        msgv[j] = (char *)malloc(sizeof(**msgv) * (word_size + 1));
        memcpy(msgv[j], &buf[i - word_size], word_size);
        msgv[j][word_size] = '\0';
        ++i;
    }
    msgv[j] = (void*)0;
    *p_msgv = msgv;
    *p_msgc = msgc;
    return 0;
}
