add_library(
    fileHandler
    fileHandler.hpp
    fileHandler.cpp
)

add_executable(quipass main.cpp)

target_link_libraries(fileHandler webview)
target_link_libraries(fileHandler base64)
target_link_libraries(fileHandler aes256)

target_link_libraries(quipass fileHandler)
target_include_directories(quipass PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")