/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 17:44:12 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/27 19:21:25 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H

# include <sys/wait.h>
# include <pwd.h>
# include "defs.h"
# include "libft.h"
# include "ft_readline.h"
# include "xfuncs.h"
# include "ft_socket.h"

# define PROJECT_NAME   "taskmaster"

extern char    		**g_cmd_names;
extern char    		**g_var_names;

typedef struct	s_str_list
{
    char			    *word;
    struct s_str_list	*next;
    struct s_str_list	*prev;
}				t_str_list;

void			shell_init(void);
void			shell_clear(void);

void			call_exec(char *source_str, int *res);
int             print_response_status(char **av, int ac);
int				is_builtin(const char *cmd);
int				ft_exit(t_str_list *lex, int res);

int			    ft_help(t_str_list *lex, int res);
int             print_action_exit_help();
int             print_action_help();
int             print_action_reload_help();
int             print_action_restart_help();
int             print_action_shutdown_help();
int             print_action_start_help();
int             print_action_status_help();
int             print_action_stop_help();

t_str_list      *init_str_list(t_str_list *prev);
t_str_list      *lexer(char *str);
char    	    *make_str(t_str_list *lex);
void	        free_one_str_list(t_str_list *list);
void		    free_all_str_list(t_str_list *list);
int             send_package(t_str_list *lex, int fd);

#endif
