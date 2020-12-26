/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_linebuf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 17:19:21 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/02 17:24:48 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"

t_buffer		g_buffer;

void	init_linebuf(t_line *line)
{
	int cols;

	cols = get_term_cols();
	g_buffer.b = str_xduplicate(line->prompt);
	g_buffer.original = str_xduplicate(g_buffer.b);
	pull_escseqs(&g_buffer.escseqs, &g_buffer.b);
	g_buffer.prompt_len = g_buffer.b.len;
	g_buffer.cpos = g_buffer.prompt_len;
	g_buffer.out_rows = g_buffer.b.len / cols + 1;
	g_buffer.out_cols = cols;
	g_buffer.out = build_bufout(g_buffer.b, cols);
}
