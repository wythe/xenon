cmake_minimum_required(VERSION 2.8)
include(${CMAKE_SOURCE_DIR}/xenonunit.txt)

add_executable(recref recref_unit.cpp)
add_test(recref recref)

add_subdirectory(itu)
add_subdirectory(xmlparse)
add_subdirectory(document)
add_subdirectory(script)

add_subdirectory(xddlunit)
