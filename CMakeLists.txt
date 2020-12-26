cmake_minimum_required(VERSION 3.1)

project(taskmaster)

add_custom_target(fclean
    COMMAND ${CMAKE_BUILD_TOOL} clean
)

add_subdirectory(libsocket)

add_subdirectory(taskmasterd)

add_subdirectory(taskmasterctl/libft)
add_subdirectory(taskmasterctl/libreadline)
add_subdirectory(taskmasterctl)
