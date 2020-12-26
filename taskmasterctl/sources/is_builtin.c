/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 21:37:10 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/25 23:07:08 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int			is_builtin(const char *cmd)
{
	if (ft_strequ(cmd, "start") || ft_strequ(cmd, "stop") ||
			ft_strequ(cmd, "restart") || ft_strequ(cmd, "exit") || 
			ft_strequ(cmd, "status") || ft_strequ(cmd, "reload") ||
			ft_strequ(cmd, "shutdown") || ft_strequ(cmd, "help"))
		return (1);
	return (0);
}
