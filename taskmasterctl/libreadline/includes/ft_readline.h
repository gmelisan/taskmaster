/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan </var/spool/mail/vladimir>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 16:35:39 by gmelisan          #+#    #+#             */
/*   Updated: 2019/08/02 23:35:16 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H

# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/ioctl.h>

# include "libft.h"
# include "ft_string.h"
# include "ft_vector.h"

# include "defs.h"
# include "line.h"
# include "xmalloc.h"
# include "str_xfuncs.h"
# include "vec_xfuncs.h"
# include "ft_errno.h"
# include "terminal.h"
# include "keys.h"
# include "bindings.h"
# include "display.h"
# include "signal_handlers.h"
# include "actions.h"
# include "history.h"
# include "history_search.h"
# include "input_loop.h"
# include "escseqs.h"
# include "loginfo.h"

# define DEBUG				0

# define VI_INSERT			1
# define VI_COMMAND			2

typedef struct	s_rlargs
{
	char		*prompt;
	t_history	**history;
	int			vi_mode;
}				t_rlargs;

char			*ft_readline(t_rlargs rlargs);

#endif
