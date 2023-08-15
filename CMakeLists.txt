cmake_minimum_required(VERSION 3.10)
project(HashServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(HashServer main.cpp)

target_link_libraries(HashServer PRIVATE httplib::httplib sqlite3)
