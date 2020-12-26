#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/28 04:59:39 by gmelisan          #+#    #+#              #
#    Updated: 2020/12/03 20:48:01 by gmelisan         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

cmake_minimum_required(VERSION 3.1)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)

project(taskmasterd)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra --debug") 

set(SOCKET_DIR ../libsocket)
set(CMAKE_PREFIX_PATH ${SOCKET_DIR})

find_library(PTHREAD_LIBRARY pthread)

set(SOURCES 
    src/main.cpp 
    src/Jobs.cpp
    src/Logger.cpp
    src/Server.cpp
    src/JobsManager.cpp)

include_directories(
    include
    yaml-cpp/include
    ${SOCKET_DIR}/includes
    )

add_subdirectory(yaml-cpp)

link_directories(${SOCKET_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARY} yaml-cpp socket)
