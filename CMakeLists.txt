cmake_minimum_required(VERSION 3.22)
project(Quipass VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

set(WEBVIEW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/webview")
set(BASE64_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-base64")
set(AES256_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/Aes256/src")

add_subdirectory(include)
add_subdirectory(src)

add_custom_target(update_html
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/updateHTML.py
    COMMENT "Updating HTML source"
)
#add_dependencies(quipass update_html)