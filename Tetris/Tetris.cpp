//=========================================================
// Project	Win32 ����˹�������
// File		Tetris.cpp ������
// Update	2019.08
// Author	miller https://github.com/similarduckweed
// Download ���أ�https://github.com/similarduckweed/Tetris/
// Tools	Visual Studio 2017
//=========================================================

#include"Tetris.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int nCmdShow)
{
	static TCHAR	szClsName[] = _T("Tetris");
	HWND  hWnd;
	MSG   msg;

	InitCommonControls(); //��ʼ��ͨ�ÿؼ����

	//��������ʱ��ʼ������
	hInst = hInstance;
	RandomBlock(&ii, &jj, &cc);
	//x = 4; y = -1;
	iLevel = 1; iSorce = 0;
	iLayer = 0; iSorceMax = TetrisIni(0);
	hDcAlert = CreateCompatibleDC(NULL);

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
			_T("This program requires Windows NT!"),
			_T("RegisterClass ERROR"),
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
	case WM_CREATE:
		GameUI(hWnd); //��������ؼ�
		GameStart(hWnd); //���ݳ�ʼ��
		//�������ֳ�ʼ��
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE1;
		hMenu = GetMenu(hWnd);
		MusicItem(IDM_MUSIC1);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawMatrix(hdc); //������Ϸ��
		DrawTetris(hdc, 0, 0, 0); //���Ʒ���
		DrawNext(hdc); //����Ԥ����
		BitBlt(hdc, 40, 200, 160, 50, hDcAlert, 0, 0, SRCCOPY);//������ʾ��
		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		DropTetris(hWnd);
		return 0;
	case WM_DRAWITEM:
		DrawButton(lParam); //���Ʒ���ť
		return TRUE;
	case WM_KEYDOWN:
		switch ((int)wParam)
		{
		case VK_LEFT:
			if (!bStart) break;
			if (CheckBlock(x - 1, y))
			{
				x--;
				hdc = GetDC(hWnd);
				DrawTetris(hdc, -1, 0, 0);
				ReleaseDC(hWnd, hdc);
			}

			break;
		case VK_RIGHT:
			if (!bStart) break;
			if (CheckBlock(x + 1, y))
			{
				x++;
				hdc = GetDC(hWnd);
				DrawTetris(hdc, 1, 0, 0);
				ReleaseDC(hWnd, hdc);
			}

			break;
		case VK_UP:
			if (!bStart || y < 0) break;
			RankTetris(hWnd);
			break;
		case VK_DOWN:
			if (!bStart) break;
			DropTetris(hWnd);
			break;
		case VK_SPACE:
			GamePause(hWnd);
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_STOP:
			if (y == -1) break;
			GameOver(hWnd);
			break;
		case IDM_NEW:
			GameStart(hWnd);
			GamePause(hWnd);
			break;
		case IDM_PAUSE:
			GamePause(hWnd);
			break;
		case IDM_EXIT:
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)AboutDlgProc);
			break;
		case IDM_HELP:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, (DLGPROC)HelpDlgProc);
			break;
		case IDM_MUSIC:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, (DLGPROC)MusicDlgProc);
			break;
		case IDM_MUSIC1:
			MusicItem(IDM_MUSIC1);
			break;
		case IDM_MUSIC2:
			MusicItem(IDM_MUSIC2);
			break;
		case IDM_MUSICOFF:
			MusicItem(IDM_MUSICOFF);
			break;
		case IDM_LEFT:
			SendMessage(hWnd, WM_KEYDOWN, VK_LEFT, 0);
			SetFocus(hWnd);
			break;
		case IDM_RIGHT:
			SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, 0);
			SetFocus(hWnd);
			break;
		case IDM_UP:
			SendMessage(hWnd, WM_KEYDOWN, VK_UP, 0);
			SetFocus(hWnd);
			break;
		case IDM_DOWN:
			SendMessage(hWnd, WM_KEYDOWN, VK_DOWN, 0);
			SetFocus(hWnd);
			break;
		}
		return 0;
	case WM_CLOSE:
		if (iSorce > iSorceMax) iSorceMax = iSorce;
		TetrisIni(iSorceMax);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
//���ڶԻ��򴰿ڹ���
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return FALSE;
}
//�����Ի��򴰿ڹ���
BOOL CALLBACK HelpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return FALSE;
}
//�������ֶԻ��򴰿ڹ���
BOOL CALLBACK MusicDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		if (iMusicBG == IDR_WAVE1) CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		if (iMusicBG == IDR_WAVE2) CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
		if (!bMusicOn) CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			break;
		case IDC_RADIO2:
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
			break;
		case IDC_RADIO3:
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			break;
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
			{
				bMusicOn = TRUE;
				iMusicBG = IDR_WAVE1;
				MusicItem(IDM_MUSIC1);
				if (bStart) TetrisMusic(TRUE);
			}
			if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
			{
				bMusicOn = TRUE;
				iMusicBG = IDR_WAVE2;
				MusicItem(IDM_MUSIC2);
				if (bStart) TetrisMusic(TRUE);
			}
			if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
			{
				bMusicOn = FALSE;
				MusicItem(IDM_MUSICOFF);
				TetrisMusic(FALSE);
			}
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return FALSE;
}
//���ֲ˵�����
void MusicItem(int iCheckID)
{
	CheckMenuRadioItem(hMenu, IDM_MUSIC1, IDM_MUSICOFF, iCheckID, MF_BYCOMMAND);

	if (iCheckID == IDM_MUSIC1)
	{
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE1;
		if (bStart) TetrisMusic(TRUE);
	}
	else if (iCheckID == IDM_MUSIC2)
	{
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE2;
		if (bStart) TetrisMusic(TRUE);
	}
	else
	{
		bMusicOn = FALSE;
		TetrisMusic(FALSE);
	}

	return;
}
//��������
void TetrisMusic(BOOL start)
{
	if (start && bMusicOn && !bGOver)
		PlaySound(MAKEINTRESOURCE(iMusicBG), NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);
	else
		PlaySound(NULL, NULL, NULL);
	return;
}
//��ʼ����Ϸ����
void GameStart(HWND hWnd)
{
	int m, n;
	HDC hdc;
	hDcBitmap = CreateCompatibleDC(NULL);

	//��ʼ��Matrix����(����)
	for (m = 0; m  < 19; m ++)
	{
		for (n = 0; n < 10; n++)
		{
			Matrix[m][n] = -1; //Matrix[m][n]����ʼ��ɫֵΪ-1
		}
	}

	//��ʼ�������λ��(�м俪ʼ����)
	x = 4; y = -1;

	//��ʼ����־����
	bStart = FALSE;
	bGOver = FALSE;

	//��ʼ����Ϸ��
	hdc = GetDC(hWnd);
	DrawMatrix(hdc);
	ReleaseDC(hWnd, hdc);

	//��ʼ���Ʒ���
	iLevel = 1;
	wsprintf(szBuffer, _T("%i"), iLevel);
	SetWindowText(hLevel, szBuffer); //����

	iLayer = 0;
	wsprintf(szBuffer, _T("%i"), iLayer);
	SetWindowText(hLayer, szBuffer); //����

	if (iSorce > iSorceMax) iSorceMax = iSorce;
	wsprintf(szBuffer, _T("%i"), iSorceMax);
	SetWindowText(hSorceMax, szBuffer); //��߷�

	iSorce = 0;
	wsprintf(szBuffer, _T("%i"), iSorce);
	SetWindowText(hSorce, szBuffer); //�÷�

	//״̬����ʾ
	SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)TEXT("�ո����ʼ/��ͣ"));
	SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)TEXT("������ϼ����Ʊ��Σ����ҡ��¼������ƶ�"));

	//�����ʾDC�ڵ�λͼ
	hDcAlert = CreateCompatibleDC(NULL);
}
//��ͣ/������Ϸ
void GamePause(HWND hWnd)
{
	if (bGOver) return;
	if (bStart)
	{
		KillTimer(hWnd, 1);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)_T("��Ϸ��ͣ"));
		TetrisMusic(FALSE);
	}
	else
	{
		SetTimer(hWnd, 1, (550 - 55 * iLevel), NULL);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)_T("��Ϸ�� ^-^"));
		TetrisMusic(TRUE);
	}
	bStart = !bStart;
	return;
}
//��Ϸ����
void GameOver(HWND hWnd)
{
	HDC	hdc;

	KillTimer(hWnd, 1); //�Ƴ���ǰ��SetTimer���õĶ�ʱ��
	SelectObject(hDcAlert, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9)));
	hdc = GetDC(hWnd);
	BitBlt(hdc, 40, 200, 160, 50, hDcAlert, 0, 0, SRCCOPY);//������ʾ��
	ReleaseDC(hWnd, hdc);
	SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)_T("��Ϸ����"));

	bGOver = TRUE;
	x = 4; y = -1;

	TetrisMusic(FALSE);
	return;
}
//��INI�ļ��ж�/д��߷�
int TetrisIni(int value)
{
	int iSorceMax = 0;
	TCHAR szBuffer[10];
	TCHAR szIniFile[100];

	SHGetSpecialFolderPath(0, szIniFile, CSIDL_APPDATA, 0);
	lstrcat(szIniFile, _T("\\Win32Tetris"));
	CreateDirectory(szIniFile, NULL);
	lstrcat(szIniFile, _T("\\Config.ini"));

	if (value == 0)
	{//��
		iSorceMax = GetPrivateProfileInt(_T("Win32Tetris"), _T("SorceMax"), 0, szIniFile);
	}
	else
	{//д
		wsprintf(szBuffer, _T("%i"), value);
		WritePrivateProfileString(_T("Win32Tetris"), _T("Contact"), _T("www.EasyIDE.cn"), szIniFile);
		WritePrivateProfileString(_T("Win32Tetris"), _T("SorceMax"), szBuffer, szIniFile);
	}

	return iSorceMax;
}
//��Matrix�е����ݻ�����Ϸ��
void DrawMatrix(HDC hdc)
{
	int m, n;
	//����Matrix����
	for (m = 0; m < 19; m++)
	{
		for (n = 0; n < 10; n++)
		{
			if (Matrix[m][n] == -1)
			{
				PatBlt(hdc, 20 + n * 20, 40 + m * 20, 20, 20, BLACKNESS); //��(20,40)��(20+18*20,40+9*20)��ÿ�����Ϊ20*20�ľ��Ρ�
			}
			else
			{
				SelectObject(hDcBitmap, LoadBitmap(hInst, MAKEINTRESOURCE(120 + Matrix[m][n])));
				BitBlt(hdc, 20 + n * 20, 40 + m * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);
			}
		}
	}
	return;
}
//���Ʒ���
void DrawTetris(HDC hdc,int iOffsetX,int iOffsetY,int iRank)
{
	int iX, iY, iJ; //�ƶ����

	if (y == -1)return;

	//�������ƽ�ƹ������ԭ�������λ��
	if (iOffsetX!=0 || iOffsetY!=0)
	{
		//ÿ�����鶼����4����(С����)���
		//��һ����a
		iX = x + Tetris[i][j].a.x - iOffsetX;
		iY = y + Tetris[i][j].a.y - iOffsetY;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		//�ڶ�����b
		iX = x + Tetris[i][j].b.x - iOffsetX;
		iY = y + Tetris[i][j].b.y - iOffsetY;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		//��������c
		iX = x + Tetris[i][j].c.x - iOffsetX;
		iY = y + Tetris[i][j].c.y - iOffsetY;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		//���ĸ���d
		iX = x + Tetris[i][j].d.x - iOffsetX;
		iY = y + Tetris[i][j].d.y - iOffsetY;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);
	}

	//���������ת�������ԭ���ķ���λ��
	if (iRank != 0)
	{
		//?
		iJ = j == 0 ? 3 : j - 1;

		iX = x + Tetris[i][iJ].a.x;
		iY = y + Tetris[i][iJ].a.y;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		iX = x + Tetris[i][iJ].b.x;
		iY = y + Tetris[i][iJ].b.y;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		iX = x + Tetris[i][iJ].c.x;
		iY = y + Tetris[i][iJ].c.y;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);

		iX = x + Tetris[i][iJ].d.x;
		iY = y + Tetris[i][iJ].d.y;
		PatBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, BLACKNESS);
	}

	//�����·���
	//SelectObject :��һ������(λͼ�����ʡ���ˢ��)ѡ��ָ�����豸�������µĶ������ͬһ���͵��϶���
	/*
	HGDIOBJ SelectObject(
		HDC hdc,          // handle to DC �豸��������(Ҫ������豸��������)
		HGDIOBJ hgdiobj   // handle to object ѡ��Ҫ����Ķ���ľ��
	);
	*/
	SelectObject(hDcBitmap, LoadBitmap(hInst, MAKEINTRESOURCE(120 + c)));

	iX = x + Tetris[i][j].a.x;
	iY = y + Tetris[i][j].a.y;
	BitBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = x + Tetris[i][j].b.x;
	iY = y + Tetris[i][j].b.y;
	BitBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = x + Tetris[i][j].c.x;
	iY = y + Tetris[i][j].c.y;
	BitBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = x + Tetris[i][j].d.x;
	iY = y + Tetris[i][j].d.y;
	BitBlt(hdc, 20 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);
	
	return;
}
//����Ԥ����
void DrawNext(HDC hdc)
{
	RECT	rc;
	int	iX, iY;

	//Ԥ��������ɫ
	rc.left = 240; rc.top = 40;
	rc.right = 340; rc.bottom = 140;
	FillRect(hdc, &rc, CreateSolidBrush(RGB(0, 0, 0)));

	//����Ԥ������
	SelectObject(hDcBitmap, LoadBitmap(hInst, MAKEINTRESOURCE(120 + cc)));

	iX = 1 + Tetris[ii][jj].a.x; iY = 1 + Tetris[ii][jj].a.y;
	BitBlt(hdc, 240 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = 1 + Tetris[ii][jj].b.x; iY = 1 + Tetris[ii][jj].b.y;
	BitBlt(hdc, 240 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = 1 + Tetris[ii][jj].c.x; iY = 1 + Tetris[ii][jj].c.y;
	BitBlt(hdc, 240 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	iX = 1 + Tetris[ii][jj].d.x; iY = 1 + Tetris[ii][jj].d.y;
	BitBlt(hdc, 240 + iX * 20, 40 + iY * 20, 20, 20, hDcBitmap, 0, 0, SRCCOPY);

	return;
}
//��������µķ�������
void RandomBlock(int * l, int * m, int * n)
{
	srand((unsigned)time(NULL));

	*l = rand() % 7;
	*m = rand() % 4;
	*n = rand() % 7;

	return;
}
//����Խ����
BOOL CheckBlock(int x, int y)
{
	int m, n;
	//��ʼλ�ò����������ƶ�
	if (y == -1) return false;

	//��Ϸ��Խ����
	if (Tetris[i][j].a.x + x < 0 || Tetris[i][j].a.x + x>9 || Tetris[i][j].a.y + y > 18) return FALSE;
	if (Tetris[i][j].b.x + x < 0 || Tetris[i][j].b.x + x>9 || Tetris[i][j].b.y + y > 18) return FALSE;
	if (Tetris[i][j].c.x + x < 0 || Tetris[i][j].c.x + x>9 || Tetris[i][j].c.y + y > 18) return FALSE;
	if (Tetris[i][j].d.x + x < 0 || Tetris[i][j].d.x + x>9 || Tetris[i][j].d.y + y > 18) return FALSE;

	//�����������
	for (m = 0; m <= 18; m++)
	{
		for (n = 0; n <= 9; n++)
		{
			if (m == Tetris[i][j].a.y + y && n == Tetris[i][j].a.x + x && Matrix[m][n] != -1) return FALSE;
			if (m == Tetris[i][j].b.y + y && n == Tetris[i][j].b.x + x && Matrix[m][n] != -1) return FALSE;
			if (m == Tetris[i][j].c.y + y && n == Tetris[i][j].c.x + x && Matrix[m][n] != -1) return FALSE;
			if (m == Tetris[i][j].d.y + y && n == Tetris[i][j].d.x + x && Matrix[m][n] != -1) return FALSE;
		}
	}

	return TRUE;
}
//��������
void DropTetris(HWND hWnd)
{
	BOOL bReDraw;
	int l, m, n;
	HDC hdc;

	//�����һ������λ
	if (CheckBlock(x, y + 1))
	{ //��������

		hdc = GetDC(hWnd); //���ڿͻ��������DC�ľ��
		if (y == -1)
		{
			//���ݷ��鲢�ػ�Ԥ����
			i = ii; j = jj; c = cc;
			RandomBlock(&ii, &jj, &cc);
			DrawNext(hdc);

			//�����·���
			y++;
			DrawTetris(hdc, 0, 0, 0);
		}
		else
		{
			//�����·���
			y++;
			DrawTetris(hdc, 0, 1, 0);
		}
		ReleaseDC(hWnd, hdc);
	}
	else
	{ //��������

		//�ڳ�ʼλ��Խ���������Ϸ
		if (y == -1)
		{
			GameOver(hWnd);
			return;
		}

		//���浱ǰ����
		Matrix[Tetris[i][j].a.y + y][Tetris[i][j].a.x + x] = c;
		Matrix[Tetris[i][j].b.y + y][Tetris[i][j].b.x + x] = c;
		Matrix[Tetris[i][j].c.y + y][Tetris[i][j].c.x + x] = c;
		Matrix[Tetris[i][j].d.y + y][Tetris[i][j].d.x + x] = c;

		//����ɨ�� �������
		for (l = 18; l >= 0; l--)
		{
			bReDraw = TRUE;
			for (m = 0; m <= 9; m++)
			{
				if (Matrix[l][m] == -1)
				{
					bReDraw = FALSE;
					//break;
				}
			}
			if (bReDraw)//��⵽��l��Ϊ����
			{
				//���¼Ʒ���
				iLayer += 1;
				wsprintf(szBuffer, _T("%i"), iLayer);
				SetWindowText(hLayer, szBuffer); //����
				iSorce += 10;
				wsprintf(szBuffer, _T("%i"), iSorce);
				SetWindowText(hSorce, szBuffer); //�÷�
				if (iSorce >= (iLevel * 50 + 50)*iLevel / 2) //����
				{
					iLevel++;
					wsprintf(szBuffer, _T("%i"), iLevel);
					SetWindowText(hLevel, szBuffer);
					SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)_T("��ϲ������"));
				}
				else
				{
					wsprintf(szBuffer, _T("��ϲ�ӷ֣������´��������� %i �֣�"), (iLevel * 50 + 50)*iLevel / 2 - iSorce);
					SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)szBuffer);
				}
				//ɾ����������
				for (m = 0; m <= 9; m++) Matrix[l][m] = -1;
				//������������
				for (n = l; n > 0; n--)
				{
					for (m = 0; m <= 9; m++)
					{
						Matrix[n][m] = Matrix[n - 1][m];
						Matrix[n - 1][m] = -1;
					}
				}
				//�ػ���Ϸ��
				hdc = GetDC(hWnd);
				DrawMatrix(hdc);
				ReleaseDC(hWnd, hdc);
				l++; //���¼�鵱ǰ��
			}
		}

		//���÷���λ��
		x = 4; y = -1;
	}
}
//������ת
void RankTetris(HWND hWnd)
{
	HDC	hdc;
	if (j == 3)
	{
		j = 0;
		if (CheckBlock(x, y))
		{
			hdc = GetDC(hWnd);
			DrawTetris(hdc, 0, 0, 1);
			ReleaseDC(hWnd, hdc);
		}
		else
		{
			j = 3;
		}
	}
	else
	{
		j++;
		if (CheckBlock(x, y))
		{
			hdc = GetDC(hWnd);
			DrawTetris(hdc, 0, 0, 1);
			ReleaseDC(hWnd, hdc);
		}
		else
		{
			j--;
		}
	}
}
//�ڿؼ�����ʾĬ���豸����
void SetNormalText(HWND hwnd, BOOL redraw)
{
	static HFONT	hFont;
	LOGFONT		lfBuffer;

	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lfBuffer), &lfBuffer);
	hFont = CreateFontIndirect(&lfBuffer);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, (LPARAM)redraw);

	return;
}

//�����Ի���Ϣ
void DrawButton(LPARAM lParam)
{
	HDC			hdc;
	LPDRAWITEMSTRUCT	pdis;
	HPEN			hpen;

	pdis = (LPDRAWITEMSTRUCT)lParam; //�����Ի���Ϣ�Ľṹ��
	hdc = pdis->hDC;
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 128));
	SelectObject(hdc, hpen);
	switch (pdis->CtlID)
	{
	case IDM_UP:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem, WM_ERASEBKGND, (WPARAM)pdis->hDC, NULL);
		if (pdis->itemState & ODS_SELECTED)
		{
			MoveToEx(hdc, 13, 1, NULL);
			LineTo(hdc, 1, 25);
			LineTo(hdc, 25, 25);
			LineTo(hdc, 13, 1);
			MoveToEx(hdc, 12, 3, NULL);
			LineTo(hdc, 2, 24);
		}
		else
		{
			MoveToEx(hdc, 12, 1, NULL);
			LineTo(hdc, 1, 24);
			LineTo(hdc, 24, 24);
			LineTo(hdc, 12, 1);
			MoveToEx(hdc, 13, 2, NULL);
			LineTo(hdc, 25, 25);
			LineTo(hdc, 2, 25);
		}
		break;
	case IDM_DOWN:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem, WM_ERASEBKGND, (WPARAM)pdis->hDC, NULL);
		if (pdis->itemState & ODS_SELECTED)
		{
			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, 13, 25);
			LineTo(hdc, 25, 1);
			LineTo(hdc, 1, 1);
			MoveToEx(hdc, 1, 2, NULL);
			LineTo(hdc, 24, 2);
		}
		else
		{
			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, 13, 24);
			LineTo(hdc, 24, 1);
			LineTo(hdc, 1, 1);
			MoveToEx(hdc, 25, 2, NULL);
			LineTo(hdc, 13, 25);
			LineTo(hdc, 1, 2);
		}
		break;
	case IDM_LEFT:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem, WM_ERASEBKGND, (WPARAM)pdis->hDC, NULL);
		if (pdis->itemState & ODS_SELECTED)
		{
			MoveToEx(hdc, 1, 13, NULL);
			LineTo(hdc, 25, 25);
			LineTo(hdc, 25, 1);
			LineTo(hdc, 1, 13);
			MoveToEx(hdc, 2, 13, NULL);
			LineTo(hdc, 24, 2);
		}
		else
		{
			MoveToEx(hdc, 1, 12, NULL);
			LineTo(hdc, 24, 23);
			LineTo(hdc, 24, 1);
			LineTo(hdc, 1, 12);
			MoveToEx(hdc, 1, 13, NULL);
			LineTo(hdc, 25, 25);
			LineTo(hdc, 25, 1);
		}
		break;
	case IDM_RIGHT:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem, WM_ERASEBKGND, (WPARAM)pdis->hDC, NULL);
		if (pdis->itemState & ODS_SELECTED)
		{
			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, 1, 25);
			LineTo(hdc, 25, 12);
			LineTo(hdc, 1, 1);
			MoveToEx(hdc, 2, 24, NULL);
			LineTo(hdc, 2, 2);
			LineTo(hdc, 24, 13);
		}
		else
		{
			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, 1, 24);
			LineTo(hdc, 24, 12);
			LineTo(hdc, 1, 1);
			MoveToEx(hdc, 1, 25, NULL);
			LineTo(hdc, 25, 13);
		}
		break;
	}
	DeleteObject(hpen);

	return;
}

//��Ϸ����ؼ�
void GameUI(HWND hWnd)
{
	int	stWidth[] = { 100,-1 };
	HWND	hTemp;

	//������
	CreateToolbarEx(
		hWnd,  //hwnd �����ھ��
		WS_VISIBLE | WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CCS_NODIVIDER,  //ws ���������
		IDR_TOOLBAR1,  //wID ���������Ӵ���ID
		6,  //nBitmaps λͼ�а������ٸ���ťСͼ��
		hInst,  //hBMInst ����λͼ��Դ��ģ����
		IDB_BITMAP1,  //wBMID λͼ��ԴID
		tbButtons,  //lpButtons
		8,  //iNumButtons ��ť����
		16,  //dxButton ������ť���
		16,  //dyButton ������ť�߶�
		16,  //dxBitmap ������ťСͼ��Ŀ��
		16,  //dyBitmap ������ťСͼ��ĸ߶�
		sizeof(TBBUTTON));  //uStructSize

	//״̬��
	hStatus = CreateStatusWindow(
		WS_CHILD | WS_VISIBLE,
		NULL,
		hWnd,
		IDS_STATUSBAR);
	SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)(stWidth));

	//�������µĺ���
	CreateWindow(
		_T("static"),  //lpClassName ����
		NULL,  //lpWindowName ����
		WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
		1, 24,
		358, 1,
		hWnd,  //hWndParent
		NULL,  //hMenu
		hInst,
		NULL);  //lpParam  ����Ĳ���

	//��Ϸ�������
	CreateWindow(
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | SS_GRAYFRAME,
		17, 37,
		206, 386,
		hWnd,
		NULL,
		hInst,
		NULL);

	//Ԥ���������
	CreateWindow(
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | SS_GRAYFRAME,
		237, 37,
		106, 106,
		hWnd,
		NULL,
		hInst,
		NULL);

	//����
	hTemp = CreateWindow(
		_T("static"),
		_T("���� :"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		240, 190,
		48, 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp, FALSE);
	hLevel = CreateWindowEx(
		WS_EX_STATICEDGE,
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_CENTERIMAGE,
		280, 186,
		60, 18,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hLevel, FALSE);

	//����
	hTemp = CreateWindow(
		_T("static"),
		_T("���� :"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		240, 220,
		36, 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp, FALSE);
	hLayer = CreateWindowEx(
		WS_EX_STATICEDGE,
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_CENTERIMAGE,
		280, 216,
		60, 18,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hLayer, FALSE);

	//�÷�
	hTemp = CreateWindow(
		_T("static"),
		_T("�÷� :"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		240, 250,
		36, 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp, FALSE);
	hSorce = CreateWindowEx(
		WS_EX_STATICEDGE,
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_CENTERIMAGE,
		280, 246,
		60, 18,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hSorce, FALSE);

	//��߷�
	hTemp = CreateWindow(
		_T("static"),
		_T("��߷�"),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		240, 280,
		36, 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp, FALSE);
	hSorceMax = CreateWindowEx(
		WS_EX_STATICEDGE,
		_T("static"),
		NULL,
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_CENTERIMAGE,
		280, 276,
		60, 18,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hSorceMax, FALSE);

	//����� ��
	CreateWindow(
		_T("button"),
		NULL,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		280, 340,
		26, 26,
		hWnd,
		(HMENU)IDM_UP,
		hInst,
		NULL);

	//����� ��
	CreateWindow(
		_T("button"),
		NULL,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		280, 370,
		26, 26,
		hWnd,
		(HMENU)IDM_DOWN,
		hInst,
		NULL);

	//����� ��
	CreateWindow(
		_T("button"),
		NULL,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		253, 355,
		26, 26,
		hWnd,
		(HMENU)IDM_LEFT,
		hInst,
		NULL);

	//����� ��
	CreateWindow(
		_T("button"),
		NULL,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		307, 355,
		26, 26,
		hWnd,
		(HMENU)IDM_RIGHT,
		hInst,
		NULL);

	hTemp = CreateWindow(
		_T("static"),
		_T("����Դ�����������ҳ��https://github.com/similarduckweed/Tetris/"),
		WS_CHILD | WS_VISIBLE | SS_LEFT | WS_DISABLED,
		18, 433,
		500, 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp, FALSE);


	return;
}
