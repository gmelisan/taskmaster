/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:45:11 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/27 19:21:04 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char    		**g_cmd_names;
char    		**g_var_names;

static void
init_readline(t_rlargs *rlargs, t_history **history)
{
    *history = ft_xmemalloc(sizeof(**history));
    history_load(*history, DEFAULT_HIST_PATH);
    rlargs->prompt = ft_strdup("\e[0;36m>\e[m ");
    rlargs->history = history;
    rlargs->vi_mode = 0;
}

static void
readline_clear(char **prompt, t_history *history)
{
    ft_strdel(prompt);
    history_clear(history);
}

static void
main_loop(int *res)
{
    t_rlargs	    rlargs;
    t_history	    *history;
	char		    *buf;

    init_readline(&rlargs, &history);
	while (1)
	{
		if (!(buf = ft_readline(rlargs)))
			break ;
		call_exec(buf, res);
		free(buf);
	}
    readline_clear(&rlargs.prompt, history);
}

int
main(int argc, char *argv[])
{
    int             res;

    shell_init();
	main_loop(&res);
	shell_clear();
	return (res);
}
