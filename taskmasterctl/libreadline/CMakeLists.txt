cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME readline)
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 11)
set(LIBFT_ROOT ../libft)
set(LIBFT libft)
set(LIBFT_A ${LIBFT}.a)
set(LIBFT_INCLUDE_DIR ${LIBFT_ROOT}/includes)

include_directories(includes)
include_directories(${LIBFT_INCLUDE_DIR})

link_directories(${LIBFT_ROOT})

file(GLOB READLINE_SRCS includes/actions.h
        includes/bindings.h
        includes/defs.h
        includes/display.h
        includes/escseqs.h
        includes/ft_errno.h
        includes/ft_readline.h
        includes/history.h
        includes/history_search.h
        includes/input_loop.h
        includes/keys.h
        includes/line.h
        includes/loginfo.h
        includes/signal_handlers.h
        includes/str_xfuncs.h
        includes/terminal.h
        includes/vec_xfuncs.h
        includes/xmalloc.h
        sources/actions/backward_char.c
        sources/actions/backward_delete_char.c
        sources/actions/backward_line.c
        sources/actions/backward_word.c
        sources/actions/beginning_of_line.c
        sources/actions/clear_screen.c
        sources/actions/delete_char.c
        sources/actions/end_of_line.c
        sources/actions/forward_char.c
        sources/actions/forward_line.c
        sources/actions/forward_word.c
        sources/actions/kill_line.c
        sources/actions/next_history.c
        sources/actions/overwrite_mode.c
        sources/actions/previous_history.c
        sources/actions/reset_history_search.c
        sources/actions/reset_line.c
        sources/actions/reverse_search_history.c
        sources/actions/self_insert.c
        sources/actions/transpose_chars.c
        sources/actions/transpose_words.c
        sources/actions/unix_line_discard.c
        sources/actions/unix_word_rubout.c
        sources/actions/yank.c
        sources/bindings/ft_bind.c
        sources/bindings/bind_keys.c
        sources/bindings/bindings.c
        sources/bindings/vi_bind_keys.c
        sources/display/clear_linebuf.c
        sources/display/init_linebuf.c
        sources/display/move_cursor.c
        sources/display/redisplay.c
        sources/display/update_line.c
        sources/escseqs.c
        sources/ft_errno.c
        sources/ft_readline.c
        sources/history.c
        sources/history_search.c
        sources/input_loop.c
        sources/loginfo.c
        sources/main_readline.c
        sources/signal_handlers.c
        sources/str_xfuncs1.c
        sources/str_xfuncs2.c
        sources/terminal.c
        sources/vec_xfuncs.c
        sources/vi_input_loop.c
        sources/xmalloc.c
        )

add_library(${PROJECT_NAME} ${READLINE_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC includes)

target_link_libraries(${PROJECT_NAME} ft curses)
