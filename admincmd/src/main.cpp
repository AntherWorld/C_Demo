#include <windows.h>

int main() {
    // cmd.exe 的路径
    LPCSTR cmdPath = "C:\\\\Windows\\\\System32\\\\cmd.exe";
    // 要执行的命令，这里以“/C dir”为例，“/C”参数告诉cmd执行字符串并退出
    LPCSTR cmdArgs = "/K c:\\users\\%username%\\Desktop\\test\\gloabl.bat";

    // 使用ShellExecute运行cmd.exe并执行特定命令
    ShellExecute(NULL, "runas", cmdPath, cmdArgs, NULL, SW_SHOWDEFAULT);
    return 0;
}