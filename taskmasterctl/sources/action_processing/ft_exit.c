/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 17:41:34 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/18 16:25:28 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	check_arg_format(const char *av)
{
	const char	*tmp;

	tmp = av;
	while (*tmp)
	{
		if (!ft_isdigit(*(tmp++)))
		{
			ft_putstr_fd(PROJECT_NAME ": exit: ", 2);
			ft_putstr_fd(av, 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(255);
		}
	}
}

int			ft_exit(t_str_list *lex, int res)
{
	if (lex)
	{
		if (lex->next)
		{
			ft_putendl_fd(PROJECT_NAME ": exit: too many arguments", 2);
			exit(1);
		}
		check_arg_format(lex->word);
		exit((char)ft_atoi(lex->word));
	}
	exit(res);
}
