/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 05:45:54 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/07/09 20:56:43 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *src, int j)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	if (!src)
		return (NULL);
	while (src[len] != '\0')
		len++;
	if (j < len)
		len = j;
	str = (char*)ft_xmalloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
