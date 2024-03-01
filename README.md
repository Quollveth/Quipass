# Quipass - A Simple Password Manager

Quipass is a password manager written in C++, featuring a GUI using webview. It stores all data in a JSON format encrypted with AES256.

This project served as a learning experience, in using proper build systems and third-party libraries.

## Libraries Used

- [Webview](https://github.com/webview/webview)
- [Base64](https://github.com/ReneNyffenegger/cpp-base64)
- [AES256](https://github.com/Urban82/Aes256)

## Building the Project

Quipass is built using CMake. The CMakeLists file in the project root facilitates the configuration of library paths. Building should be initiated from this location.

The GUI is powered by webview, with HTML, CSS, and JavaScript bundled for convenience. Prior to building, ensure these files are updated and copied into `src/main.cpp`. Although an automated Python script is provided for this purpose (currently commented out), manual copying suffices. Python is only required for this step and not for running the project.

## License

This project is released under the MIT license.
