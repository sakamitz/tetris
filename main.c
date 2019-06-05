/*
 * 项目：Tetris
 * 文件名：main.c
 * 概览：总体控制模块
 * -------------
 * 负责功能：
 *      用户事件捕捉与分配
 *      界面的实时显示刷新
 */

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"    // 需传递用户输入信息
#include "tetris.h"   // 需控制方块形态
#include "flow.h"     // 需控制游戏流程
#include "layout.h"   // 需更新界面
#include "interact.h" // 需用到交互界面


// 游戏控制键及菜单快捷键定义
#define Key_Rotate      0x57   // W
#define Key_Drop        0x53   // S
#define Key_Left        0x41   // A
#define Key_Right       0x44   // D
#define Key_Hold        0x45   // E
#define Key_SpeedUp     0x46   // F
#define Key_Pause       0x50   // P
#define Key_Save        0x4B   // K
#define Key_Restart     0x4E   // N
#define Key_BackToMain  0x4D   // M
#define Key_Help        0x4C   // L

double winwidth, winheight;


// 用户输入处理函数
void CharEventProcess(char ch);
void MouseEventProcess(int x, int y, int button, int event);
void KeyboardEventProcess(int key, int event);

// 界面显示刷新函数
void SetBackground();
void display();

void Main()
{
    SetWindowTitle("Tetris");
    SetWindowSize(5, 7);
    InitGraphics();

    // 背景颜色定义
    DefineColor("Background", 7/255.0, 54/255.0, 66/255.0);
    // 使用预定义的按钮和文本框样式
    usePredefinedButtonColors(4);
    usePredefinedTexBoxColors(6);

    winwidth  = GetWindowWidth();
    winheight = GetWindowHeight();

    // 注册用户输入事件回调函数
    registerCharEvent(CharEventProcess);
    registerMouseEvent(MouseEventProcess);
    registerKeyboardEvent(KeyboardEventProcess);

    display(); // 显示刷新
}

/*
 * 函数名：CharEventProcess
 * -------------
 * 获取用户输入，主要用于文本框
 */
void CharEventProcess(char ch)
{
    // 只有在显示文本框时才传递输入，避免多余字符
    tetGameStatus gs = getGameStatu();
    if (gs == ON_SAVE || gs == ON_LOAD || gs == ON_RECORD)
    {
        uiGetChar(ch);
        display();
    }
}

/*
 * 函数名：MouseEventProcess
 * -------------
 * 捕捉鼠标事件，用于按钮、菜单的响应
 */
void MouseEventProcess(int x, int y, int button, int event)
{
    uiGetMouse(x, y, button, event);
    display();
}

/*
 * 函数名：KeyboardEventProcess
 * -------------
 * 捕捉键盘事件，用于游戏控制及快捷键处理
 */
void KeyboardEventProcess(int key, int event)
{
    uiGetKeyboard(key, event);

    // 一次按下/抬起中，键位只处理一次
    if (event == KEY_UP)
    {
        display();
        return;
    }

    // 当前为暂停状态且按下继续键，则继续游戏
    if (getGameStatu() == ON_PAUSE && key == Key_Pause)
        gameResume();

    // 当前正在进行游戏
    else if (getGameStatu() == ON_PLAYING)
    {
        switch (key)
        {
            case Key_Left:
                move(TM_LEFT);
                break;

            case Key_Right:
                move(TM_RIGHT);
                break;

            case Key_Rotate:
                rotate();
                break;

            case Key_Drop:
                drop();
                break;

            case Key_SpeedUp:
                move(TM_DOWN);
                break;

            case Key_Hold:
                hold();
                break;

            case Key_Pause:
                gamePause();
                break;

            case Key_Restart:
                gamePause();
                setGameStatu(ON_RESTART);
                break;

            case Key_BackToMain:
                gamePause();
                setGameStatu(ON_BACKTOMAIN);
                break;

            case Key_Save:
                gamePause();
                setGameStatu(ON_SAVE);
                break;

            case Key_Help:
                gamePause();
                setGameStatu(ON_HELP);

            default:
                break;
        }
    }

    display();
}

/*
 * 函数名：SetBackground
 * -------------
 * 使用自定义的颜色作为背景
 */
void SetBackground()
{
    SetPenColor("Background");
    drawRectangle(0, 0, winwidth, winheight, 1);
}

void display()
{
    // 标题文字
    static char* header = "Tetris - by C Programming Group 23";

    SetBackground();
    dispFrame();    // 绘制框架
    dispStat();     // 绘制游戏数据

    // 根据当前游戏状态确定绘制的窗口
    switch (getGameStatu())
    {
        // 主菜单界面
        case ON_MAIN:
            dispHeader(header);
            dispStartWindow();
            break;

        // 游戏中
        case ON_PLAYING:
            dispGameMenu();
            dispAllBlocks();
            break;

        // 暂停状态
        case ON_PAUSE:
            dispGameMenu();
            dispAllBlocks();
            MsgBoxResume();
            break;

        // 排行榜
        case ON_RANKING:
            dispHeader(header);
            dispRankWindow();
            break;

        // 帮助界面
        case ON_HELP:
            dispHeader(header);
            dispHelpWindow();
            break;

        // 载入存档
        case ON_LOAD:
            dispHeader(header);
            MsgBoxLoad();
            break;

        // 保存存档
        case ON_SAVE:
            dispGameMenu();
            dispAllBlocks();
            MsgBoxSave();
            break;

        // 操作成功
        case ON_SUCCESS:
            dispHeader("Congratulations!");
            dispAllBlocks();
            MsgBoxSuccess();
            break;

        // 读取存档失败
        case ON_LOADFAIL:
            dispHeader(header);
            MsgBoxLoadFail();
            break;

        // 保存（分数或存档）失败
        case ON_SAVEFAIL:
            dispHeader("Oops! Something is wrong");
            dispAllBlocks();
            MsgBoxSaveFail();
            break;

        // 是否重新开始
        case ON_RESTART:
            dispGameMenu();
            dispAllBlocks();
            MsgBoxRestart();
            break;

        // 是否回到主菜单
        case ON_BACKTOMAIN:
            dispGameMenu();
            dispAllBlocks();
            MsgBoxBackToMain();
            break;

        // 游戏结束
        case ON_GAMEOVER:
            dispHeader("Game Over!");
            dispAllBlocks();
            MsgBoxGameOver();
            break;

        // 保存用户名及分数记录
        case ON_RECORD:
            dispHeader("Game Over!");
            dispAllBlocks();
            MsgBoxRecord();
            break;

        default:
            break;
    }
}