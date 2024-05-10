cmake_minimum_required(VERSION 3.0.0)
project(code)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
enable_testing()
add_executable(code
src/account.cpp
src/main.cpp
src/order.cpp
src/train.cpp
src/utility.cpp
src/valid.cpp)