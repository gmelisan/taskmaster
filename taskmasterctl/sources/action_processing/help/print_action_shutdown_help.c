/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action_shutdown_help.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 17:41:34 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/18 16:25:28 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int
print_action_shutdown_help()
{
    ft_putendl("shutdown        Stop running processes and shutdown daemon.");
    return (0);
}
