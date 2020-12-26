/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 01:25:17 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/27 01:50:45 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void		shell_clear(void)
{
	ft_free_double_ptr_arr((void ***)&g_cmd_names);
	ft_free_double_ptr_arr((void ***)&g_var_names);
}
