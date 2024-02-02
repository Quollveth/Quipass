add_library(
    webview
    webview.h
    webview.cc
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk REQUIRED gtk+-3.0 IMPORTED_TARGET)
pkg_check_modules(webkit REQUIRED webkit2gtk-4.0 IMPORTED_TARGET)
target_link_libraries(webview PkgConfig::gtk PkgConfig::webkit)

target_include_directories(webview PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")