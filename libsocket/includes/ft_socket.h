/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 15:24:52 by ggwin-go          #+#    #+#             */
/*   Updated: 2020/10/18 15:50:29 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SOCKET_H
 #define FT_SOCKET_H

 #include <sys/socket.h>
 #include <sys/un.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <string.h>

 #define SOCKET_PATH "/tmp/taskmaster.sock"

typedef struct  sockaddr_un t_sockaddr_un;

 #ifdef __cplusplus
 extern "C" {
 #endif

int             encode_message(const char *msg, char **p_buf, int *p_size);
int             decode_message(const char *buf, char ***p_msgv, int *p_msgc);

 #ifdef __cplusplus
 }
 #endif

#endif
