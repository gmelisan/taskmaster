cmake_minimum_required(VERSION 3.1)

project(taskmasterctl)

set(CMAKE_C_STANDARD 11)

set(OUTPUT_FOLDER ./)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})

set(LIBFT_DIR ./libft)
set(READLINE_DIR ./libreadline)
set(SOCKET_DIR ../libsocket)

include_directories(
	includes
	${LIBFT_DIR}/includes
	${READLINE_DIR}/includes
	${SOCKET_DIR}/includes
	)

set(SOURCES
	sources/action_processing/ft_exit.c
	sources/action_processing/ft_help.c
	sources/action_processing/help/print_action_exit_help.c
	sources/action_processing/help/print_action_help.c
	sources/action_processing/help/print_action_reload_help.c
	sources/action_processing/help/print_action_restart_help.c
	sources/action_processing/help/print_action_shutdown_help.c
	sources/action_processing/help/print_action_start_help.c
	sources/action_processing/help/print_action_status_help.c
	sources/action_processing/help/print_action_stop_help.c
	sources/action_processing/print_response_status.c
	sources/action_processing/call_exec.c
	sources/lexer.c
	sources/is_builtin.c
	sources/main.c
	sources/print_error.c
	sources/shell_clear.c
	sources/shell_init.c
	sources/init_str_list.c
	sources/free_all_str_list.c
	sources/free_one_str_list.c
	sources/send_package.c
	sources/make_str.c
	)

link_directories(${LIBFT_DIR})
link_directories(${READLINE_DIR})
link_directories(${SOCKET_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} readline curses socket ft)
