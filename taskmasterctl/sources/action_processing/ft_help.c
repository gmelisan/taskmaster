/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_help.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 17:41:34 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/18 16:25:28 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int
print_topic(const char *action)
{
	if (ft_strequ(action, "exit"))
		return (print_action_exit_help());
	else if (ft_strequ(action, "help"))
		return (print_action_help());
	else if (ft_strequ(action, "reload"))
		return (print_action_reload_help());
	else if (ft_strequ(action, "restart"))
		return (print_action_restart_help());
	else if (ft_strequ(action, "shutdown"))
		return (print_action_shutdown_help());
	else if (ft_strequ(action, "start"))
		return (print_action_start_help());
	else if (ft_strequ(action, "status"))
		return (print_action_status_help());
	else if (ft_strequ(action, "stop"))
		return (print_action_stop_help());
	ft_putendl_fd("*** No help on such topic", STDERR);
	return (127);	
}

int
ft_help(t_str_list *lex, int res)
{
	if (lex)
	{
		if (lex->next)
		{
			ft_putendl_fd("*** No help on such topic", STDERR);
			return (127);
		}
		return print_topic(lex->word);
	}
	ft_printf("default commands (type help <topic>):\n"
			"=====================================\n"
			"%-8s  %-8s  %-8s  %-8s\n"
			"%-8s  %-8s  %-8s  %-8s\n",
			"exit", "help", "reload", "restart",
			"shutdown", "start", "status", "stop");
	return (0);
}
