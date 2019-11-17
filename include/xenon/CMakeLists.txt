cmake_minimum_required(VERSION 3.12)

add_compile_options(-Wno-unused-parameter)
add_executable(dummy dummy.cpp ${CMAKE_CURRENT_BINARY_DIR}/xddl.h)

target_include_directories(dummy PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
    OUTPUT xddl.h
    COMMAND xspx -H xddl.h -S foo.cpp ${CMAKE_SOURCE_DIR}/src/xddl.xspx
    BYPRODUCTS foo.c
    COMMENT generating xddl.h
    DEPENDS xspx ${CMAKE_SOURCE_DIR}/src/xddl.xspx
)
