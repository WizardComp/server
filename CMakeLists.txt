cmake_minimum_required(VERSION 3.12)
project(server C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(server main.c monitor_impl.c  socket.c socket.h )
target_link_libraries(server pthread)