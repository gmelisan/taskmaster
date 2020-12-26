/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 18:30:19 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/01 07:05:18 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execinfo.h>
#include "xmalloc.h"

void			die(void)
{
	term_restore();
	ft_fdprintf(STDERR, "Fatal: out of virtual memory.\n");
	ft_fdprintf(g_logfd, "Fatal: out of virtual memory.\n");
	exit(1);
}

void		*xmalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		die();
	return (ptr);
}

void		*ft_xmemalloc(size_t size)
{
	void *ptr;

	ptr = ft_memalloc(size);
	if (!ptr)
		die();
	return (ptr);
}
