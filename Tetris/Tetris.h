//=========================================================
// Project	Win32 ����˹�������
// File		Tetris.h ������ͷ�ļ�
// Update	2019.08
// Author	miller https://github.com/similarduckweed/
// Download ���أ�https://github.com/similarduckweed/Tetris/
// Tools	Visual C++ 6.0 & Editplus 3
//=========================================================

// Լ�����ڱ������У�
// [����]��ָ���ĸ�С������϶��ɵ�һ���󷽿�
// [��Ϸ��]��ָ��������ĺ�ɫ����
// [Ԥ����]��ָ��ʾ��һ������ĺ�ɫ����

// �ͻ���˵����
// С���� 20*20(����)
// ��Ϸ��С������� 10*19(��)  ����ԭ�� 20,40(����)
// Ԥ����С������� 5*5(��)  ����ԭ�� 240,40(����)

// �Ʒ��㷨
// ����Ϊnʱ��������ʱ�����Ϊ550-55n����
// ÿ��һ�м�10��
// ����Ϊnʱӵ�еķ���Ϊ((n-1)*50+50)*(n-1)/2

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <commctrl.h>
#include <shlobj.h>
#include <mmsystem.h>
#include "resource.h"
#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "comctl32.lib")



HINSTANCE hInst; //ʵ�����
HWND hStatus; //״̬�����
HMENU hMenu; //�˵����
HWND hLevel, hLayer, hSorce, hSorceMax; //�Ʒ������

static TBBUTTON tbButtons[] =
{
	{0,IDM_NEW,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
	{1,IDM_PAUSE,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
	{2,IDM_STOP,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
	{0,0,TBSTATE_ENABLED,BTNS_SEP,0,0,-1},
	{3,IDM_MUSIC,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
	{4,IDM_HELP,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
	{0,0,TBSTATE_ENABLED,BTNS_SEP,0,0,-1},
	{5,IDM_ABOUT,TBSTATE_ENABLED,BTNS_BUTTON,0,0,-1},
}; //���������ݽṹ

//����ģ�ͽṹ(7����״)
/* 
  �������  �����  �������  �������  ������  ������  ������
  �������  ��������  �������  �������  ��������  ��������  ��������
*/
/*(ÿ�������PIONT)
  0 1 2 3
0 �� �� �� ��
1 �� �� �� ��
2 �� �� �� ��
3 �� �� �� ��
*/
//�����ṹ��(��4��POINT����)
struct Block { POINT a; POINT b; POINT c; POINT d;};
// ����ṹ�����(��7�ַ��飬ÿ�ַ��������4�ֱ仯)
static Block Tetris[7][4] =
{
	//����
	//����(1)  1�ֱ仯�����нṹ��һ��
	{
		{{0,0},{0,1},{1,0},{1,1}},{{0,0},{0,1},{1,0},{1,1}},
		{{0,0},{0,1},{1,0},{1,1}},{{0,0},{0,1},{1,0},{1,1}}
	},
	//����    ��
	// ����  ����(2)  2�ֱ仯
	//     ��
	{
		{{0,0},{0,1},{1,1},{1,2}},{{1,0},{2,0},{0,1},{1,1}},
		{{0,0},{0,1},{1,1},{1,2}},{{1,0},{2,0},{0,1},{1,1}}
	},
	//  ����  ��
	//����    ����(2)  2�ֱ仯
	//       ��
	{
		{{1,0},{1,1},{0,1},{0,2}},{{0,0},{1,0},{1,1},{2,1}},
		{{1,0},{1,1},{0,1},{0,2}},{{0,0},{1,0},{1,1},{2,1}}
	},
	//      ��
	//��������  ��(2)  2�ֱ仯
	//      ��
	//      ��
	{
		{{1,0},{1,1},{1,2},{1,3}},{{0,0},{1,0},{2,0},{3,0}},
		{{1,0},{1,1},{1,2},{1,3}},{{0,0},{1,0},{2,0},{3,0}}
	},
	// ��
	//������(4)  4�ֱ仯
	{
		{{1,0},{0,1},{1,1},{2,1}}, {{0,0},{0,1},{1,1},{0,2}},
		{{0,0},{1,0},{2,0},{1,1}}, {{1,0},{0,1},{1,1},{1,2}}
	},
	//��
	//������(4)  4�ֱ仯
	{
		{{0,0},{0,1},{1,1},{2,1}}, {{0,0},{1,0},{0,1},{0,2}},
		{{0,0},{1,0},{2,0},{2,1}}, {{1,0},{1,1},{0,2},{1,2}}
	},
	//  ��
	//������(4)  4�ֱ仯
	{
		{{2,0},{0,1},{1,1},{2,1}}, {{0,0},{0,1},{0,2},{1,2}},
		{{0,0},{1,0},{2,0},{0,1}}, {{0,0},{1,0},{1,1},{1,2}}
	}
};

//��������
int i, j, c; //�˶��еķ������� Tetris[i][j]��ɫֵc
int ii, jj, cc; //��һ���������� ͬ��
int x, y; //�˶��еķ�������ֵ
int Matrix[19][10]; //��Ϸ������
BOOL bStart; //��ʼ����ͣ��־
BOOL bGOver; //��Ϸ������־
HDC hDcBitmap; //װ�ط���λͼ�õ�DC
HDC hDcAlert; //װ����ʾλͼ�õ�DC
TCHAR szBuffer[50]; //�����ַ���
int iLevel, iSorce, iLayer, iSorceMax; //�Ʒֱ���
BOOL bMusicOn; //���ֿ���
int iMusicBG; //��������

//��������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetNormalText(HWND, BOOL);
void GameUI(HWND);
void DrawButton(LPARAM);
void DrawTetris(HDC, int, int, int);
void DrawNext(HDC);
void DrawMatrix(HDC);
void RandomBlock(int *, int *, int *);
BOOL CheckBlock(int, int);
void DropTetris(HWND);
void RankTetris(HWND);
void GameStart(HWND);
void GamePause(HWND);
void GameOver(HWND);
int TetrisIni(int);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK HelpDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MusicDlgProc(HWND, UINT, WPARAM, LPARAM);
void TetrisMusic(BOOL);
void MusicItem(int);