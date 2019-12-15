// DicConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "resource.h"
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <thread>
#include <io.h>
#include <iterator>
#include <string>
#include <vector>
#include <map>
using namespace std;

map<string, string> mDictory;


vector<string> find_file(string dir_path) {
	_finddata64i32_t finddata;
	vector<string> arr_file;
	long handle = _findfirst(dir_path.c_str(), &finddata);
	do
	{
		arr_file.push_back(finddata.name);
	} while (!_findnext(handle, &finddata));

	return arr_file;
}

/*
输入:一个文件夹的路径
输出:txt文件的路径
*/
vector<string> find_sub_txtfile(string sub_dir_path,map<string,string>& mDic) {
	string pattern = sub_dir_path.append("/").append("*.txt");
	vector<string> arr_txtfile_path;
	_finddata64i32_t finddata;

	long handle = _findfirst(pattern.c_str(), &finddata);
	if (handle == -1)return vector<string>();
	do
	{
		size_t pos = pattern.find_last_of("/");
		arr_txtfile_path.push_back((string(pattern.begin(), pattern.begin() + pos + 1) + finddata.name));

	} while (!_findnext(handle,&finddata));

	return arr_txtfile_path;
}

void readTxtData(string txtfile,map<string,string>& mDic) {
	ifstream fs;
	fs.open(txtfile);

	if (!fs.is_open()) {
		cout << "此文件没有打开" << endl;
		return;
	}

	string vab;
	string mean;

	while (!fs.eof())
	{
		getline(fs, vab);
		if (fs.eof()) break;
		getline(fs, mean); 
		
		mDic.insert(make_pair(vab, mean));
	}

}



map<string, string> mData;

void thread_proc(HWND hWnd, char* vab) {
	map<string, string>::iterator mapIter = mData.begin();
	for (; mapIter != mData.end(); ++mapIter)
	{
		string t_vab = (*mapIter).first;
		size_t index = t_vab.find(vab);
		if (index != -1) {
			SendDlgItemMessageA(hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)t_vab.c_str());
		}
	}
}


INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	map<string, string>::iterator mapIter;
	vector<string> txtFiles;
	int res = 0;
	char vab[256];
	switch (uMsg)
	{
	case WM_INITDIALOG:
		//hlistWndj = GetDlgItem(hWnd, IDC_LIST1);
		//PostMessageA(hlistWndj, LB_ADDSTRING, 0, (LPARAM)"hello");
		mData.clear();
		txtFiles = find_file("./*.txt");
		for (int i=0;i<txtFiles.size();++i)
		{
			readTxtData(txtFiles[i], mData);
		}


		mapIter = mData.begin();
		for (;mapIter!=mData.end();++mapIter)
		{
			string t_vab = (*mapIter).first;
			SendDlgItemMessageA(hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)t_vab.c_str());
		}

		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDCANCEL:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDC_LIST1:

			if (HIWORD(wParam) == LBN_SELCHANGE) {
				unsigned int index = SendDlgItemMessageA(hWnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				cout << index << endl;
				SendDlgItemMessageA(hWnd, IDC_LIST1, LB_GETTEXT, index, (LPARAM)vab);
				SendDlgItemMessageA(hWnd, IDC_EDIT2, WM_SETTEXT, 0, (LPARAM)mData[vab].c_str());

			}
			break;
		case IDC_EDIT1:
			SendDlgItemMessageA(hWnd, IDC_EDIT1, WM_GETTEXT, 256, (LPARAM)vab);
			cout << vab << endl;


			SendDlgItemMessageA(hWnd, IDC_LIST1, LB_RESETCONTENT, 0, 0);
			thread_proc(hWnd, vab);
			//thread th1(thread_proc, hWnd, vab);


			//th1.join();

			break;
		}
		return 1;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 1;
	}

	return 0;
}


int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd ) {

	FILE* stream1;
	AllocConsole();
	freopen_s(&stream1, "conin$", "r", stdin);
	freopen_s(&stream1, "conout$", "w", stdout);
	
	HWND hDialog = CreateDialogA(hInstance, MAKEINTRESOURCEA(IDD_DIALOG1), NULL, DlgProc);
	ShowWindow(hDialog, nShowCmd);
	UpdateWindow(hDialog);


	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	fclose(stdin);
	fclose(stdout);

	FreeConsole();



}

