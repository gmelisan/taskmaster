/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backward_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 07:16:18 by gmelisan          #+#    #+#             */
/*   Updated: 2019/07/20 07:16:30 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "actions.h"

void	backward_line(t_line *line)
{
	int w;

	w = get_screenwidth();
	if (line->cpos < w - (int)line->prompt.len)
		return ;
	line->cpos -= w;
	if (line->cpos < 0)
		line->cpos = 0;
}
