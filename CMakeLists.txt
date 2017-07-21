cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)

project(mdbrowser)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -O0 -DDEBUG -g")
set(CMAKE_C_FLAGS_DEBUG "-O0 -DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3")
set(CMAKE_C_FLAGS_RELEASE " -O3")

set(LITEBROWSER_PATH src)
set(LITEHTML_PATH litehtml)
#set(CONTAINER_PATH ${LITEHTML_PATH}/containers/linux)

pkg_check_modules(LB_LIBS REQUIRED fontconfig gdkmm-3.0 gtkmm-3.0 libcurl cairo gtksourceviewmm-3.0)

set(SOURCE  ${LITEBROWSER_PATH}/main.cpp
            ${LITEBROWSER_PATH}/htmlwidget.cpp
            ${LITEBROWSER_PATH}/container.cpp
            ${LITEBROWSER_PATH}/htmlwindow.cpp
            ${LITEBROWSER_PATH}/globals.cpp
    )

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${LITEHTML_PATH}/include 
${LB_LIBS_INCLUDE_DIRS} 
cmark/src 
cmark/extensions
${CMAKE_CURRENT_BINARY_DIR}/cmark/src
${CMAKE_CURRENT_BINARY_DIR}/cmark/extensions
)

add_subdirectory(${LITEHTML_PATH})
add_subdirectory(utils)

set(CMARK_TESTS OFF CACHE BOOL "")
set(CMARK_STATIC ON CACHE BOOL "")
add_subdirectory(cmark)

find_program(XXD_COMMAND xxd)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/master.css.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/static/master.css | xxd -i > ${CMAKE_CURRENT_BINARY_DIR}/master.css.inc)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/src/master.css.inc PROPERTIES GENERATED TRUE)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/base.css.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/static/base.css | xxd -i > ${CMAKE_CURRENT_BINARY_DIR}/base.css.inc)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/base.css.inc PROPERTIES GENERATED TRUE)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/base.html.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/static/base.html | xxd -i > ${CMAKE_CURRENT_BINARY_DIR}/base.html.inc)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/base.html.inc PROPERTIES GENERATED TRUE)

add_executable(mdbrowser 
  ${CMAKE_CURRENT_BINARY_DIR}/master.css.inc 
  ${CMAKE_CURRENT_BINARY_DIR}/base.css.inc 
  ${CMAKE_CURRENT_BINARY_DIR}/base.html.inc
  src/httploader.h 
  src/httploader.cpp
  ${SOURCE} )

target_link_libraries(mdbrowser litehtml utils libcmark-gfm_static libcmark-gfmextensions_static ${LB_LIBS_LIBRARIES})

