cmake_minimum_required(VERSION 3.3)
project(APT_assignment2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wall -pedantic")

set(SOURCE_FILES ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c ppd_coin.c)
add_executable(APT_assignment2 ${SOURCE_FILES})