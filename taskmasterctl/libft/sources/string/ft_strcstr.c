/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 23:06:44 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/04/22 23:07:55 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcstr(char *str, char *tmp, char c)
{
	if (str == NULL || tmp == NULL)
		return (0);
	while (*str && *tmp && *str != c)
	{
		if (*str != *tmp)
			return (0);
		++str;
		++tmp;
	}
	if (*str && *str == c)
		return (1);
	return (0);
}
