/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 16:54:13 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/02 23:29:26 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

int		main(void)
{
	int			exit_flag;
	char		*res;
	t_history	*history;
	t_rlargs	rlargs;

	exit_flag = 0;
	history = ft_xmemalloc(sizeof(*history));
	history_load(history, DEFAULT_HIST_PATH);
	rlargs.prompt = ft_strdup("\E[0;31mreadline #\E[m ");
	rlargs.history = &history;
	rlargs.vi_mode = 0;
	while (42)
	{
		res = ft_readline(rlargs);
		if (!res)
			break ;
		if (g_errno)
			printerr();
		else
			ft_printf("-> \"%s\"\n", res);
		ft_strdel(&res);
	}
	ft_strdel(&rlargs.prompt);
	history_clear(history);
	return (0);
}
