/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:47:28 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/04/16 18:24:14 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int ch)
{
	char *str1;

	str1 = (char*)str;
	while (*str1 != '\0')
	{
		if (*str1 == ch)
			return (str1);
		str1++;
	}
	if (*str1 == ch)
		return (str1);
	return (NULL);
}
