cmake_minimum_required(VERSION 3.16)

project(EECE2080C_tower LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EECE2080C_tower main.cpp
    stack.h stack.cpp
    hanoi.h hanoi.cpp
    queue.h queue.cpp)

include(GNUInstallDirs)
install(TARGETS EECE2080C_tower
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
