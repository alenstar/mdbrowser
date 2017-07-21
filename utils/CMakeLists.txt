# Build steps
cmake_minimum_required(VERSION 2.6)
project(utils)

#file(GLOB_RECURSE APP_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
set(UTILS_SRC_FILES 
    utils.cpp
	parser.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(
    ${PROJECT_SOURCE_DIR}/src 
    )
#add_dependencies(utils tinyxml2)
add_library(utils STATIC ${UTILS_SRC_FILES})

add_executable(example-utils example.cpp ${UTILS_SRC_FILES})
target_link_libraries(example-utils m pthread)