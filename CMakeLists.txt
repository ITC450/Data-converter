cmake_minimum_required(VERSION 3.8)
project(Picture_converter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4 -pipe")

find_package( OpenCV REQUIRED )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(SOURCE_FILES main.cpp treewalker.h treewalker.cpp)
add_executable(Picture_converter ${SOURCE_FILES})

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD 14
)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( Picture_converter ${OpenCV_LIBS} )