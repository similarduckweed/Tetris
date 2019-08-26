//=========================================================
// Project	Win32 俄罗斯方块程序
// File		Tetris.cpp 主程序
// Update	2019.08
// Author	miller https://github.com/similarduckweed
// Download 源码爱好者：http://www.NewXing.com
// Tools	Visual Studio 2017
//=========================================================

#include"Tetris.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int nCmdShow)
{
	static TCHAR	szClsName[] = TEXT("Tetris");

	HWND  hWnd;
	MSG   msg;

	//窗口类
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = NULL;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szClsName;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	//注册窗口类
	if (!RegisterClassEx(&wcex))
	{
		::MessageBox(
			NULL,
			TEXT("This program requires Windows NT!"),
			TEXT("RegisterClass ERROR"),
			MB_ICONERROR);
			return 0;
	}

	//创建主窗口
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,  //创建的窗口的扩展窗口样式
		szClsName,
		_T("俄罗斯方块Win32版"),
		WS_MINIMIZEBOX | WS_SYSMENU,  //正在创建的窗口的样式
		GetSystemMetrics(SM_CXFULLSCREEN) / 2 - 370 / 2,  //窗口的初始水平位置
		GetSystemMetrics(SM_CYFULLSCREEN) / 2 - 370 / 2,  //窗口的初始垂直位置
		370, 520,  //窗口的宽度和高度
		NULL,  //正在创建的窗口的父窗口或所有者窗口的句柄
		NULL,  //菜单的句柄
		hInstance,  //与窗口关联的模块实例的句柄
		NULL);  //指向要通过CreateStruct结构传递到窗口的值的指针

	//显示窗口和刷新客户区
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	//循环获取消息,分发消息
	while (GetMessage(&msg,NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam; //相当于return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC  hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}