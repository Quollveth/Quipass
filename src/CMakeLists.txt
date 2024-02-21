add_library(
    fileHandler
    fileHandler.hpp
    fileHandler.cpp
)

add_executable(quipass main.cpp)

target_link_libraries(quipass webview)
target_link_libraries(quipass fileHandler)
target_include_directories(quipass PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")