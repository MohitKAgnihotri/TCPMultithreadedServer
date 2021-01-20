cmake_minimum_required(VERSION 3.13)
project(tcp_client_server)

find_package (Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g0 -O2 -Wall -Werror -Wpedantic -std=c++11 -pthread")

include_directories(include)

add_executable(tcp_server
        src/tcp_server.cpp
        src/client.cpp
        src/RequestParser.cpp
        tcp_server_main.cpp
        src/message_board.cpp include/message_board.h)

add_executable(tcp_client
        tcp_client_main.cpp
        )
