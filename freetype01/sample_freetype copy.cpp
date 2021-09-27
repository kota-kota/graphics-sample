#include "GLFW/glfw3.h"
#include <iostream>

namespace {
    //! ウインドウタイトル・幅・高さ
    constexpr char* WIN_TITLE = "sample_draw";
    constexpr std::int32_t WIN_W = 1280;
    constexpr std::int32_t WIN_H = 720;
}

namespace {
    //! GLFWでエラーが発生したときにコールされるコールバック関数
    static void glfw_error_callback(int error, const char* description)
    {
        std::cerr << "[GLFW ERROR] (" << error << ") " << description << std::endl;
    }
}

int main()
{
    std::cout << "[main] app start" << std::endl;
    // GLFWでエラーが発生したときにコールされる関数を登録する
    glfwSetErrorCallback(glfw_error_callback);

    // GLFWを初期化する
    if (glfwInit() == GL_FALSE) {
        // 失敗
        std::cerr << "* glfwInit() .. NG" << std::endl;
        return 1;
    }
    std::cout << "* glfwInit() .. OK" << std::endl;

    // OpenGL ES 3.2 Core Profile を選択する
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // ウィンドウを作成する
    GLFWwindow* const window = glfwCreateWindow(WIN_W, WIN_H, WIN_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        // 失敗
        std::cerr << "* glfwCreateWindow() .. NG" << std::endl;
        glfwTerminate();
        return 1;
    }
    std::cout << "* glfwCreateWindow() .. OK (0x" << window << ")" << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "[main] app end" << std::endl;

    return 0;
}