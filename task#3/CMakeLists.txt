cmake_minimum_required(VERSION 3.10)
project(task3)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)

pkg_check_modules(GLIB glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
link_libraries(${GLIB_LIBRARIES})

set(SOURCE_FILES main.c)
add_executable(task3 ${SOURCE_FILES})
