cmake_minimum_required(VERSION 3.2)
project(ieee754)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

FILE(GLOB SOURCE_FILES src/*.cpp)
add_executable(ieee754 ${SOURCE_FILES})
target_link_libraries(ieee754 ${GTKMM_LIBRARIES})
