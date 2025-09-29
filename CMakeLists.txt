cmake_minimum_required(VERSION 3.10)
project(neighbor-technical-project)

set(CMAKE_CXX_STANDARD 17)
find_package(Drogon REQUIRED)

add_executable(neighbor_technical_project main.cpp algo.cpp)
target_link_libraries(neighbor_technical_project Drogon::Drogon)