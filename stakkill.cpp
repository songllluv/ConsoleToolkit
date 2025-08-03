#include <Windows.h>

// 全局钩子句柄
HHOOK g_hMouseHook = NULL;

// 鼠标钩子回调函数
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {
            // 鼠标左键点击时执行的操作
            // 这里添加解除机房管理的实际代码
			      system("taskkill /im student.exe /f /im smonitor.exe /t");
        }
    }
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

// 设置鼠标钩子
void SetHook() {
    g_hMouseHook = SetWindowsHookEx(
        WH_MOUSE_LL,        // 低级鼠标钩子类型
        MouseHookProc,       // 回调函数
        GetModuleHandle(NULL), // 当前模块句柄
        0                    // 线程ID (0表示全局钩子)
    );
    
    if (g_hMouseHook == NULL) {
        MessageBox(NULL, "钩子设置失败!", "错误", MB_ICONERROR);
    }
}

// 主函数
int main() {
    // 设置系统钩子
    SetHook();

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 卸载钩子
    UnhookWindowsHookEx(g_hMouseHook);
    return 0;
}
