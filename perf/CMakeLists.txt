cmake_minimum_required(VERSION 3.11)

add_executable(decodeperf decode.cpp)
target_link_libraries(decodeperf PUBLIC xenon)

add_executable(loadperf loadperf.cpp)
target_link_libraries(loadperf PUBLIC xenon)
