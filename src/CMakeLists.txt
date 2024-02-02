add_library(
    uiHandler
    uiHandler.hpp
    uiHandler.cpp
)

add_executable(quipass main.cpp)

target_link_libraries(uiHandler webview)
target_link_libraries(quipass webview)
target_link_libraries(quipass uiHandler)
target_include_directories(quipass PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")