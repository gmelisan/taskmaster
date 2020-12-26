/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loginfo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 23:04:13 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/02 23:30:36 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGINFO_H
# define LOGINFO_H

# include <stdarg.h>
# include <execinfo.h>
# include "ft_readline.h"

# define LOGNAME		"./log_readline"
# define BT_BUFSIZE		32

void	logopen(void);
void	logclose(void);
void	loginfo(char *format, ...);
void	loginfo_line(t_line *line);
void	loginfo_backtrace(void);

extern int	g_logfd;

#endif
