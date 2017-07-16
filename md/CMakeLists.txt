# Build steps
cmake_minimum_required(VERSION 2.6)
project(md)

#file(GLOB_RECURSE APP_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
set(MD_SRC_FILES 
    md.cpp
    utils.cpp
		parser.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
#add_dependencies(md tinyxml2)
add_library(md STATIC ${MD_SRC_FILES})

add_executable(mdexample example.cpp ${MD_SRC_FILES})
target_link_libraries(mdexample m pthread)