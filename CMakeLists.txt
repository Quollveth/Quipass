cmake_minimum_required(VERSION 3.22)
project(Quipass VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_subdirectory(libs)
add_subdirectory(src)

add_custom_target(update_html
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/updateHTML.py
    COMMENT "Updating HTML source"
)
add_dependencies(quipass update_html)