/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 15:49:00 by ggwin-go          #+#    #+#             */
/*   Updated: 2018/11/30 19:50:45 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int i;

	i = 0;
	if (s != NULL && f != NULL)
		while (s[i] != '\0' && s[i] >= 0 && s[i] <= 127)
		{
			(*f)(i, &s[i]);
			i++;
		}
}
