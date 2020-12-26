/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 22:41:23 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/27 15:21:01 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int
print_response(char *action, char **av, int ac)
{
	if (ft_strequ(av[0], "ok") || ft_strequ(av[0], "Success"))
		ft_putstr("\e[0;32m");
	else
		ft_putstr("\e[0;31m");
    if (ft_strequ(action, "status") && ft_strequ(av[0], "ok"))
        print_response_status(av, ac);
    else
    {
        for (int i = 0; i < ac; ++i)
            ft_printf("%s ", av[i]);
        ft_printf("\n");
    }
	ft_putstr("\e[m");
    return (0);
}

static int
get_response(int fd, char *action, int *res)
{
    char    **av;
    char    *buf;
    int     len;
    int     ac;
    int     rc;

    if ((rc=read(fd, (char *)&len, sizeof(int))) < 0)
    {
        ft_putendl_fd("read error", STDERR);
        *res = 125;
    }
    buf = (char *)xmalloc(sizeof(int) + len);
    ft_memcpy(buf, &len, sizeof(int));
    if ((rc=read(fd, buf + sizeof(int), len)) < 0)
    {
        ft_putendl_fd("read error", STDERR);
        *res = 125;
    }
    av = (void *)0;
    if (decode_message(buf, &av, &ac) < 0)
    {
        ft_putendl_fd("decode error", STDERR);
        *res = 125;
    }
    print_response(action, av, ac);
    ft_free_double_ptr_arr((void ***)&av);
    ft_strdel(&buf);
    return (0);
}

static void
handle_command(t_str_list *lex, int *res)
{
    struct sockaddr_un addr;
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
        perror("Can't create socket");
        *res = -1;
        return;
    }
    addr.sun_family = AF_UNIX;
    ft_strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    if (connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
	{
        perror("Can't connect socket");
        *res = -1;
        return;
    }
    *res = send_package(lex, fd);
    get_response(fd, lex->word, res);
}

static void
processing_multiple_programs(t_str_list *lex, int *res)
{
    t_str_list  *cmd;
    t_str_list  *arg;

	cmd = init_str_list(NULL);
    cmd->word = ft_strdup(lex->word);
    while ((lex = lex->next))
    {
        arg = init_str_list(NULL);
        arg->word = ft_strdup(lex->word);
        cmd->next = arg;
        ft_printf("%s: ", arg->word);
        if (ft_strequ(cmd->word, "status"))
            ft_putchar('\n');
        handle_command(cmd, res);
		ft_memdel((void **)&arg->word);
        ft_memdel((void **)&arg);
    }
    ft_memdel((void **)&cmd->word);
    ft_memdel((void **)&cmd);
}

void
call_exec(char *source_str, int *res)
{
    t_str_list  *lex;
    char        *tmp;

    if (*(tmp = ft_strtrim(source_str)))
    {
        lex = lexer(source_str);
        if (is_builtin(lex->word))
        {
            if (ft_strequ(lex->word, "exit"))
                ft_exit(lex->next, *res);
            else if (ft_strequ(lex->word, "help"))
                ft_help(lex->next, *res);
            else if (lex->next && lex->next->next)
                processing_multiple_programs(lex, res);
            else
                handle_command(lex, res);
        }
        else
        {
            ft_fdprintf(STDERR, "\e[0;31m*** Unknown command: \"%s\"\e[m\n",
				source_str);
            *res = 127;
        }
        free_all_str_list(lex);
    }
    free(tmp);
}
