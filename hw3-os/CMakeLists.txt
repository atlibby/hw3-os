cmake_minimum_required(VERSION 3.23.2)
project(hw3_os C)

set(CMAKE_C_STANDARD 11)

add_executable(hw3_os mtcount.atlibby.akirscht.c
                      pthreads-example-simpler.c)
