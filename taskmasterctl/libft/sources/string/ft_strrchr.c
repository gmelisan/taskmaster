/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 17:56:56 by ggwin-go          #+#    #+#             */
/*   Updated: 2018/11/29 20:51:58 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int ch)
{
	char	*str1;
	int		i;
	int		k;

	i = 0;
	k = 0;
	str1 = (char*)str;
	while (str1[i] != '\0' && str1[i] >= 0 && str1[i] <= 127)
	{
		if (str1[i] == ch)
			k = i;
		i++;
	}
	if (ch == '\0')
		return (&str1[i]);
	if (str1[k] == ch)
		return (&str1[k]);
	else
		return (NULL);
}
