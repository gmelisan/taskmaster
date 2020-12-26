/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggwin-go <ggwin-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 17:25:18 by ggwin-go          #+#    #+#             */
/*   Updated: 2019/08/26 14:24:01 by ggwin-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void
remove_dupicate_arguments(t_str_list *head)
{
    t_str_list  *ptr1;
    t_str_list  *ptr2;
    t_str_list  *dup;

    if (!head || !head->next)
        return ;
    ptr1 = head->next;
    while (ptr1 != NULL && ptr1->next != NULL)
    {
        ptr2 = ptr1;
        while (ptr2->next != NULL)
        {
            if (ft_strequ(ptr1->word, ptr2->next->word))
            {
                dup = ptr2->next;
                ptr2->next = ptr2->next->next;
                free_one_str_list(dup);
            }
            else
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
}

static char *
lexer_find_symbol(char *str, char c)
{
    while (*str)
    {
        if (c != '\'' && *str == '\\')
        {
            ++str;
            if (*str)
                ++str;
            else
                return (str);
        }
        else if (*str == c)
            return (str + 1);
        else
            ++str;
    }
    return (str);
}

static size_t
lexer_check_word(char *str, t_str_list **lex)
{
    char    *begin;

    begin = str;
    while (*str && !ft_isspace(*str))
    {
        if (*str == '"')
            str = lexer_find_symbol(++str, '"');
        else if (*str == '\'')
            str = lexer_find_symbol(++str, '\'');
        else if (*str == '\\')
        {
            ++str;
            if (*str)
                ++str;
            else
                break ;
        }
        else
            ++str;
    }
    (*lex)->word = ft_strndup(begin, str - begin);
    return (str - begin);
}

static char *
lexer_get_token(char *str, t_str_list **lex)
{
    size_t	    i;

    i = lexer_check_word(str, lex);
    return (str + i);
}

t_str_list *
lexer(char *buf)
{
	t_str_list	*lex;

	lex = init_str_list(NULL);
	while (*buf)
	{
		if (ft_isspace(*buf) && ++buf)
			continue ;
		buf = lexer_get_token(buf, &lex);
		lex->next = init_str_list(lex);
		lex = lex->next;
	}
	lex = lex->prev;
	free(lex->next);
	lex->next = NULL;
    while (lex->prev)
		lex = lex->prev;
    remove_dupicate_arguments(lex);
	return (lex);
}
