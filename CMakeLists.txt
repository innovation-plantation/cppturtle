cmake_minimum_required(VERSION 3.14)
project(cppturtle)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(turtlecpp main.cpp Turtle.cpp Turtle.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(turtlecpp ${GTKMM_LIBRARIES} Threads::Threads)
