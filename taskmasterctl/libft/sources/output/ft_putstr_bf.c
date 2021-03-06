/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_bf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 20:45:04 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/02/23 21:06:25 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			ft_putstr_bf(char *str, int nb)
{
	static char buf[2000];
	static int	i;

	if (nb == 1 || i == 1999)
	{
		write(1, buf, i);
		return ;
	}
	while (*str)
	{
		buf[i] = *str;
		++i;
		if (i == 1999)
		{
			write(1, buf, 1999);
			i = 0;
		}
		str++;
	}
}
