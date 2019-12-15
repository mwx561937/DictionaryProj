#include "resource.h"
#include <windows.h>
#include <thread>
#include <iostream>

using namespace std;

DWORD WINAPI thread_proc(void* param) {
	//cout << "helloworld" << endl;
	for (int i = 0; i < 10000; ++i)
	{
		//SendDlgItemMessageA(*(HWND*)param, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"hello");
		//PostMessageA(*(HWND*)param, LB_ADDSTRING, 0, (LPARAM)"hello");
	}
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HWND hList;
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//hList = GetDlgItem(hWnd, IDC_LIST1);
		//SendDlgItemMessageA(hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"hello");
		break;
	}
	case WM_LBUTTONDOWN:
		CreateThread(0, 0, thread_proc,(void*)&hWnd, 0, 0);
		cout << "clickme" << endl;
		return 1;
	case WM_SIZE:
		cout << "resize" << endl;
		return 1;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 1;
	}

	return 0;
}


int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	AllocConsole();

	FILE* f;
	freopen_s(&f, "conin$", "r", stdin);
	freopen_s(&f, "conout$", "w", stdout);

	HWND hDialog = CreateDialog(hInstance, MAKEINTRESOURCEA(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(hDialog, nShowCmd);
	UpdateWindow(hDialog);

	MSG msg;
	while (GetMessage(&msg,0,0,0))
	{
		if(msg.message == WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CloseHandle(hDialog);

	FreeConsole();
	return 0;
}