cmake_minimum_required(VERSION 3.1)

project(socket)

set(CMAKE_C_STANDARD 11)

include_directories(includes)

file(GLOB SOCKET_SRCS includes/ft_socket.h
        sources/encode_message.c
        sources/decode_message.c
        )

add_library(${PROJECT_NAME} ${SOCKET_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC includes)
