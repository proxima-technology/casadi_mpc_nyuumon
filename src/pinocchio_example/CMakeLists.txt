cmake_minimum_required(VERSION 3.4)

project(myproject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(pinocchio REQUIRED)
find_package(casadi REQUIRED)

add_executable(get_casadi_fn_using_pinocchio src/get_casadi_fn_using_pinocchio.cpp)

target_link_libraries(get_casadi_fn_using_pinocchio pinocchio::pinocchio casadi)
