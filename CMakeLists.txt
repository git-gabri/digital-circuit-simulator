cmake_minimum_required(VERSION 3.0.0)
project(digital_circuit_sim)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -pedantic -s -O2")

#include(CTest)
#enable_testing()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include") 

add_executable(simulator main.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/include/circuit.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/include/console.cpp)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
