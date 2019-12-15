cmake_minimum_required(VERSION 3.14)
project(ForkProcess C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lcurl")

add_executable(ForkProcess main.c)
target_link_libraries(ForkProcess curl)