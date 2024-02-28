#Webview
#https://github.com/webview/webview

set(WEBVIEW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/webview")

add_library(
    webview
    "${WEBVIEW_PATH}/webview.h"
    "${WEBVIEW_PATH}/webview.cc"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk REQUIRED gtk+-3.0 IMPORTED_TARGET)
pkg_check_modules(webkit REQUIRED webkit2gtk-4.0 IMPORTED_TARGET)
target_link_libraries(webview PkgConfig::gtk PkgConfig::webkit)

target_include_directories(webview PUBLIC "${WEBVIEW_PATH}")

#cpp-base64
#https://github.com/ReneNyffenegger/cpp-base64

set(BASE64_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cpp-base64")

add_library(
    base64
    "${BASE64_PATH}/base64.h"
    "${BASE64_PATH}/base64.cpp"
)

target_include_directories(base64 PUBLIC "${BASE64_PATH}")