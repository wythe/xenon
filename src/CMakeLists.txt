cmake_minimum_required(VERSION 3.12)

add_custom_command(
    OUTPUT xddl.cpp
    COMMAND xspx -S xddl.cpp ${CMAKE_SOURCE_DIR}/src/xddl.xspx
    COMMENT generating xddl.cpp
    DEPENDS xspx ${CMAKE_SOURCE_DIR}/src/xddl.xspx
)

include_directories(lua-5.1.4/src)

add_library(xenon SHARED
    lua-5.1.4/src/lua_all.cpp
    xml_parser_base.cpp 
    message.cpp
    xenon.cpp
    xddl.cpp
)
