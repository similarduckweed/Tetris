//=========================================================
// Project	Win32 俄罗斯方块程序
// File		Tetris.h 主程序头文件
// Update	2019.08
// Author	miller https://github.com/similarduckweed/
// Download 下载：https://github.com/similarduckweed/Tetris/
// Tools	Visual C++ 6.0 & Editplus 3
//=========================================================

// 约定，在本程序中：
// [方块]是指由四个小方格组合而成的一个大方块
// [游戏区]是指方块下落的黑色区域
// [预览区]是指显示下一个方块的黑色区域

// 客户区说明：
// 小方格 20*20(像素)
// 游戏区小方格矩阵 10*19(个)  坐标原点 20,40(像素)
// 预览区小方格矩阵 5*5(个)  坐标原点 240,40(像素)

// 计分算法
// 级别为n时方块下落时间节拍为550-55n毫秒
// 每满一行加10分
// 级别为n时拥有的分数为((n-1)*50+50)*(n-1)/2

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <commctrl.h>
#include <shlobj.h>
#include <mmsystem.h>
#include "resource.h"
#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "comctl32.lib")



HINSTANCE hInst; //实例句柄
HWND hStatus; //状态栏句柄
HMENU hMenu; //菜单句柄
HWND hLevel, hLayer, hSorce, hSorceMax; //计分器句柄

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
}; //工具栏数据结构

//方块模型结构(7种形状)
/* 
  ◇◆◆◇  ◇◇◇◇  ◆◆◇◇  ◇◆◆◇  ◇◆◇◇  ◇◇◆◇  ◆◇◇◇
  ◇◆◆◇  ◆◆◆◆  ◇◆◆◇  ◆◆◇◇  ◆◆◆◇  ◆◆◆◇  ◆◆◆◇
*/
/*(每个方块的PIONT)
  0 1 2 3
0 ◇ ◇ ◇ ◇
1 ◇ ◇ ◇ ◇
2 ◇ ◇ ◇ ◇
3 ◇ ◇ ◇ ◇
*/
//申明结构体(含4个POINT类型)
struct Block { POINT a; POINT b; POINT c; POINT d;};
// 定义结构体变量(有7种方块，每种方块最多有4种变化)
static Block Tetris[7][4] =
{
	//□□
	//□□(1)  1种变化，所有结构都一样
	{
		{{0,0},{0,1},{1,0},{1,1}},{{0,0},{0,1},{1,0},{1,1}},
		{{0,0},{0,1},{1,0},{1,1}},{{0,0},{0,1},{1,0},{1,1}}
	},
	//□□    □
	// □□  □□(2)  2种变化
	//     □
	{
		{{0,0},{0,1},{1,1},{1,2}},{{1,0},{2,0},{0,1},{1,1}},
		{{0,0},{0,1},{1,1},{1,2}},{{1,0},{2,0},{0,1},{1,1}}
	},
	//  □□  □
	//□□    □□(2)  2种变化
	//       □
	{
		{{1,0},{1,1},{0,1},{0,2}},{{0,0},{1,0},{1,1},{2,1}},
		{{1,0},{1,1},{0,1},{0,2}},{{0,0},{1,0},{1,1},{2,1}}
	},
	//      □
	//□□□□  □(2)  2种变化
	//      □
	//      □
	{
		{{1,0},{1,1},{1,2},{1,3}},{{0,0},{1,0},{2,0},{3,0}},
		{{1,0},{1,1},{1,2},{1,3}},{{0,0},{1,0},{2,0},{3,0}}
	},
	// □
	//□□□(4)  4种变化
	{
		{{1,0},{0,1},{1,1},{2,1}}, {{0,0},{0,1},{1,1},{0,2}},
		{{0,0},{1,0},{2,0},{1,1}}, {{1,0},{0,1},{1,1},{1,2}}
	},
	//□
	//□□□(4)  4种变化
	{
		{{0,0},{0,1},{1,1},{2,1}}, {{0,0},{1,0},{0,1},{0,2}},
		{{0,0},{1,0},{2,0},{2,1}}, {{1,0},{1,1},{0,2},{1,2}}
	},
	//  □
	//□□□(4)  4种变化
	{
		{{2,0},{0,1},{1,1},{2,1}}, {{0,0},{0,1},{0,2},{1,2}},
		{{0,0},{1,0},{2,0},{0,1}}, {{0,0},{1,0},{1,1},{1,2}}
	}
};

//变量申明
int i, j, c; //运动中的方块数据 Tetris[i][j]颜色值c
int ii, jj, cc; //下一个方块数据 同上
int x, y; //运动中的方块坐标值
int Matrix[19][10]; //游戏区矩阵
BOOL bStart; //开始、暂停标志
BOOL bGOver; //游戏结束标志
HDC hDcBitmap; //装载方格位图用的DC
HDC hDcAlert; //装载提示位图用的DC
TCHAR szBuffer[50]; //缓冲字符串
int iLevel, iSorce, iLayer, iSorceMax; //计分变量
BOOL bMusicOn; //音乐开关
int iMusicBG; //背景音乐

//函数申明
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