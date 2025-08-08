#include <Windows.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>

// 强制终止进程（不等待资源释放）
bool ForceTerminateProcessByName(const std::wstring& processName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32W pe = { sizeof(pe) };
	bool found = false;

	if (Process32FirstW(hSnapshot, &pe)) {
		do {
			if (_wcsicmp(pe.szExeFile, processName.c_str()) != 0)
				continue;

			HANDLE hProcess = OpenProcess(
			                      PROCESS_TERMINATE,  // 移除了SYNCHRONIZE权限
			                      FALSE,
			                      pe.th32ProcessID
			                  );

			if (hProcess) {
				// 直接终止，不等待退出确认
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
				found = true;
			}
		} while (Process32NextW(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return found;
}
// 全局钩子句柄
HHOOK g_hMouseHook = NULL;

// 鼠标钩子回调函数
int t=0;
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_RBUTTONDOWN) {
			// 鼠标左键点击时执行的操作
			// 这里添加解除机房管理的实际代码
				ForceTerminateProcessByName(L"student.exe");
				ForceTerminateProcessByName(L"smonitor.exe");
			
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
	
		exit(-1); 
	}
}

// 主函数
int main() {
	// 设置系统钩子
	SetHook();
	std::cout<<"已开启，右键触发！";
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
