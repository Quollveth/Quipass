#include <uiHandler.hpp>

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
#else
int main() {
#endif
    webview::webview window = initializeUI();

    window.run();
}