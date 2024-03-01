#Webview
#https://github.com/webview/webview

#cpp-base64
#https://github.com/ReneNyffenegger/cpp-base64

#Aes256
#https://github.com/Urban82/Aes256

add_library(
    webview
    "${WEBVIEW_PATH}/webview.h"
    "${WEBVIEW_PATH}/webview.cc"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk REQUIRED gtk+-3.0 IMPORTED_TARGET)
pkg_check_modules(webkit REQUIRED webkit2gtk-4.0 IMPORTED_TARGET)
target_link_libraries(webview PkgConfig::gtk PkgConfig::webkit)

add_library(
    base64
    "${BASE64_PATH}/base64.h"
    "${BASE64_PATH}/base64.cpp"
)

add_library(
    aes256
    "${AES256_PATH}/aes256.hpp"
    "${AES256_PATH}/aes256.cpp"
)

target_include_directories(webview PUBLIC "${WEBVIEW_PATH}")
target_include_directories(base64 PUBLIC "${BASE64_PATH}")
target_include_directories(aes256 PUBLIC "${AES256_PATH}")