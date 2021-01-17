cmake_minimum_required(VERSION 3.13)
project(tcp_client_server)

find_package (Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g3 -O0 -pg -std=c++11 -pthread")

include_directories(include)

add_executable(tcp_server
        src/tcp_server.cpp
        src/client.cpp
        src/RequestParser.cpp
        main.cpp
        src/message_board.cpp include/message_board.h src/topic.cpp include/topic.h src/post.cpp include/post.h)

add_executable(tcp_client
        src/tcp_client.cpp
        src/client.cpp
        client_example.cpp
        )
