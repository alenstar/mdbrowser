cmake_minimum_required(VERSION 3.5)
project(MaDown)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/AST.cpp
        src/AST.h
        src/Parser.cpp
        src/Parser.hpp
        src/Render.cpp
        src/Render.h
        src/Madown.h
        src/Lexer.cpp
        src/Lexer.h
        src/TextStream.cpp
        src/TextStream.h
        src/util.h
        src/Visitor.h
        )

find_program(XXD_COMMAND xxd)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/base.css.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/static/base.css | xxd -i > ${CMAKE_CURRENT_SOURCE_DIR}/src/base.css.inc)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/base.css.inc PROPERTIES GENERATED TRUE)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/base.html.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/static/base.html | xxd -i > ${CMAKE_CURRENT_SOURCE_DIR}/src/base.html.inc)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/base.html.inc PROPERTIES GENERATED TRUE)

add_library(MaDown ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/base.html.inc ${CMAKE_CURRENT_SOURCE_DIR}/src/base.css.inc)

set(EXAMPLE example/main.cpp)

add_executable(example ${SOURCE_FILES} ${EXAMPLE})
