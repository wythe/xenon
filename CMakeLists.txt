cmake_minimum_required(VERSION 3.12)
project(xenon VERSION 2.4.0)
configure_file(config.h.in config.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(-Wall -Wextra -Werror)

if (APPLE)
add_compile_options(-fdiagnostics-color=always)
endif (APPLE)

include_directories("include")
include_directories("src")

if (APPLE)
include_directories(SYSTEM "/usr/local/include")
endif (APPLE)

add_subdirectory(xspx)
#add_subdirectory(tools)

