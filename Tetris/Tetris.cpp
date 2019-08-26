//=========================================================
// Project	Win32 ����˹�������
// File		Tetris.cpp ������
// Update	2019.08
// Author	miller https://github.com/similarduckweed
// Download Դ�밮���ߣ�http://www.NewXing.com
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

	//������
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
	//ע�ᴰ����
	if (!RegisterClassEx(&wcex))
	{
		::MessageBox(
			NULL,
			TEXT("This program requires Windows NT!"),
			TEXT("RegisterClass ERROR"),
			MB_ICONERROR);
			return 0;
	}

	//����������
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,  //�����Ĵ��ڵ���չ������ʽ
		szClsName,
		_T("����˹����Win32��"),
		WS_MINIMIZEBOX | WS_SYSMENU,  //���ڴ����Ĵ��ڵ���ʽ
		GetSystemMetrics(SM_CXFULLSCREEN) / 2 - 370 / 2,  //���ڵĳ�ʼˮƽλ��
		GetSystemMetrics(SM_CYFULLSCREEN) / 2 - 370 / 2,  //���ڵĳ�ʼ��ֱλ��
		370, 520,  //���ڵĿ�Ⱥ͸߶�
		NULL,  //���ڴ����Ĵ��ڵĸ����ڻ������ߴ��ڵľ��
		NULL,  //�˵��ľ��
		hInstance,  //�봰�ڹ�����ģ��ʵ���ľ��
		NULL);  //ָ��Ҫͨ��CreateStruct�ṹ���ݵ����ڵ�ֵ��ָ��

	//��ʾ���ں�ˢ�¿ͻ���
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	//ѭ����ȡ��Ϣ,�ַ���Ϣ
	while (GetMessage(&msg,NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam; //�൱��return 0;
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