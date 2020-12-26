/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 15:51:46 by ggwin-go          #+#    #+#             */
/*   Updated: 2018/11/26 15:59:43 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	int		i;
	char	*str;

	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[i] != '\0' && s[i] >= 0 && s[i] <= 127)
		i++;
	if (!(str = (char*)malloc(i + 1)))
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] >= 0 && s[i] <= 127)
	{
		str[i] = (*f)(s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
