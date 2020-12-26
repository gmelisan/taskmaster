/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_response_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 17:41:34 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/18 16:25:28 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

#define NAME_COLUMN "<name>"
#define PATH_COLUMN "<command>"
#define PID_COLUMN "<pid>"
#define STATUS_COLUMN "<status>"

static int
get_max_word_width(char *title, char **av, int index, int count)
{
	int	res;
	int	len;

	res = ft_strlen(title);
	for (int i = 0; i < count; ++i)
	{
		len = ft_strlen(av[index + 5 * i]);
		if (res < len)
			res = len;
	}
	return (res);
}

static void
generate_format_string(char **format, char **av, int ac, int count)
{
	int	w_name;
	int	w_path;
	int	w_pid;
	int	w_restart_count;
	int	w_status;

	w_name = get_max_word_width(NAME_COLUMN, av, 2, count);
	w_path = get_max_word_width(PATH_COLUMN, av, 3, count);
	w_pid = get_max_word_width(PID_COLUMN, av, 4, count);
	w_status = get_max_word_width(STATUS_COLUMN, av, 6, count);
	*format = ft_strrejoin("%-", ft_itoa(ft_strlen(av[1])), 2);
	*format = ft_strrejoin(*format, "s | %-", 1);
	*format = ft_strrejoin(*format, ft_itoa(w_name), 3);
	*format = ft_strrejoin(*format, "s | %-", 1);
	*format = ft_strrejoin(*format, ft_itoa(w_pid), 3);
	*format = ft_strrejoin(*format, "s | %-", 1);
	*format = ft_strrejoin(*format, ft_itoa(w_status), 3);
	*format = ft_strrejoin(*format, "s | %-", 1);
	*format = ft_strrejoin(*format, ft_itoa(w_path), 3);
	*format = ft_strrejoin(*format, "s\n", 1);
}

static char *
generate_divide_line(char *format, int len)
{
	char	*str;

	str = (char *)ft_xmalloc(len + 1);
	ft_memset(str, '-', len);
	str[len] = '\0';
	return (str);
}

static void
clearing_response(char **av, int ac)
{
	char	*tmp;
	char	**words;

	for (int i = 2; i < ac; ++i)
	{
		if (*av[i] == '\"')
		{
			tmp = av[i];
			av[i] = ft_strsub(tmp, 1, ft_strlen(tmp) - 2);
			ft_strdel(&tmp);
		}
		if ((i - 2) % 5 == 4) {
			words = ft_strsplit(av[i], ' ');
			tmp = av[i];
			av[i] = ft_strdup(words[0]);
			ft_strdel(&tmp);
			if (ft_strequ(av[i], "crashed") || ft_strequ(av[i], "exited") ||
				ft_strequ(av[i], "failed"))
			{
				av[i] = ft_strrejoin(av[i], " (", 1);
				av[i] = ft_strrejoin(av[i], words[1], 1);
				av[i] = ft_strrejoin(av[i], ")", 1);
			}
			ft_free_double_ptr_arr((void ***)&words);
		}
		else if ((i - 2) % 5 == 2 && !ft_strnequ(av[i + 2], "\"running", 8) &&
			!ft_strnequ(av[i + 2], "\"starting", 8) &&
			!ft_strnequ(av[i + 2], "\"stopping", 8))
		{
			ft_strdel(&av[i]);
			av[i] = ft_strdup("");
		}
	}
}

int
print_response_status(char **av, int ac)
{
	char	*format;
	char	*divide_str;
	char	*number;
	int		count;
	int		j;

	ft_putstr("\e[m");
	if (ac > 2 && ft_strequ(av[0], "ok") && (count = ft_atoi(av[1]))
		&& count * 5 + 2 >= ac)
	{
		clearing_response(av, ac);
		generate_format_string(&format, av, ac, count);
		j = ft_printf(format, "N", NAME_COLUMN, PID_COLUMN, STATUS_COLUMN,
			PATH_COLUMN);
		divide_str = generate_divide_line(format, j);
		ft_printf("%s\n", divide_str);
		j = 2;
		for (int i = 1; i <= count; ++i)
		{
			number = ft_itoa(i);
			ft_printf(format, number, av[j], av[j + 2], av[j + 4], av[j + 1]);
			ft_strdel(&number);
			j += 5;
		}
		ft_strdel(&format);
		ft_strdel(&divide_str);
	}
	else
		ft_putendl("Incorrect response");
	return (0);
}
