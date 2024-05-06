#include <iostream>
#include <windows.h>
#include <string>

std::string getCurrentPath() {
    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    return std::string(buffer);
}

int main() {
    // 获取当前路径
    std::string currentPath = getCurrentPath();

    // 构建 global.bat 文件路径
    std::string batFilePath = currentPath + "\\\\global.bat";

    // 使用 ShellExecute 执行 global.bat
    HINSTANCE result = ShellExecute(NULL, "open", batFilePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);

   

    return 0;
}
