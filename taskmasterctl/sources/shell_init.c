/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 01:24:52 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/27 01:50:29 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void		preliminary_check_fd(void)
{
	if (fcntl(0, F_GETFL) < -1)
		exit(0);
	if (!isatty(0))
		exit(0);
}

void		init_command_names(void)
{
	int				count;

	count = 0;
	g_cmd_names = ft_xmemalloc(sizeof(char *) * 10);
	g_cmd_names[count++] = ft_strdup("exit");
	g_cmd_names[count++] = ft_strdup("help");
	g_cmd_names[count++] = ft_strdup("reload");
	g_cmd_names[count++] = ft_strdup("shutdown");
	g_cmd_names[count++] = ft_strdup("start");
	g_cmd_names[count++] = ft_strdup("status");
	g_cmd_names[count++] = ft_strdup("stop");
	g_cmd_names[count++] = ft_strdup("reload");
	g_cmd_names[count++] = NULL;
}

void		shell_init(void)
{
	g_var_names = ft_xmemalloc(sizeof(char *));
	g_var_names[0] = NULL;
	preliminary_check_fd();
	init_command_names();
}
