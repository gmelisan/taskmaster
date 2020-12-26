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
encode_message(const char *msg, char **p_buf, int *p_size)
{
	if (!msg || !p_buf || !p_size)
		return -1;
	char *buf;
    int in_quote = 0;
	int msgsize = strlen(msg) + 1;
	int size = msgsize + sizeof(int);
	buf = (char *)malloc(sizeof(*buf) * (size));
	memcpy(buf, &msgsize, sizeof(int));
	int j = sizeof(int);
	for (int i = 0; i < msgsize - 1; ++i, ++j)
	{
        if (msg[i] == '"')
            in_quote = !in_quote;
        if (in_quote)
            buf[j] = msg[i];
        else
		    buf[j] = (msg[i] == ' ' ? '\n' : msg[i]);
	}
	buf[j] = '\n';
	*p_buf = buf;
	*p_size = size;
	return 0;
}