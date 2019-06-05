/*
 * 项目：Tetris
 * 文件名：interact.c
 * 概览：用户交互模块
 * -------------
 * 负责功能：各种对话框的定义，包括
 *      点击继续
 *      确认保存
 *      输入用户名
 *      操作成功/失败
 *      是否重新开始
 *      是否回到主界面
 *      是否保存记录
 */

#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"

#include "imgui.h"    // 需要绘制控件
#include "flow.h"     // 点击时需改变游戏流程
#include "fileIO.h"   // 某些交互界面涉及存档存取
#include "interact.h" // 本模块

#define FILL     1  // 填充
#define NOFILL   0  // 不填充

#define Max(x,y) ((x)>(y) ? (x):(y))

// 引入main模块中的表示窗口宽度及高度的变量
extern double winwidth, winheight;

// 错误的存档名
static char errName[NAMELEN];


/*
 * 函数名：MsgBox
 *
 * 功能：显示一个宽度自适应的交互式对话框，含有文本框输入（可选）
 *
 * 用法：choice = MsgBox(prompt, buttons, n, hasTextBox, strBuffer, buflen);
 *
 *   prompt :: char*     - 提示消息
 *   buttons[] :: char*  - 按钮列表
 *   n :: int            - 按钮个数
 *   hasTextBox :: int   - 是否含有文本框（0-否 1-是）
 *   strBuffer :: char*  - 文本框的输入缓冲区（如果不用文本框请设为NULL）
 *   buflen :: int       - 文本框缓冲区大小（不用文本框请设为0）
 *
 * 返回值 :: int
 *   -1    - 用户没有点击（按下并释放）按钮
 *   >=0   - 用户点击的按钮 index （在buttons[]中）
 *
 * 应用举例：
 *      快速参考：参见头文件(interact.h)
 *      具体应用：本模块(interact.c)之后的所有函数全是 MsgBox 的应用实例
 */
int MsgBox(char* prompt, char* buttons[], int n,
            int hasTextBox, char* strBuffer, int buflen)
{
    static double buttonWidth   = 0.80; // 按钮宽度
    static double buttonHeight  = 0.30; // 按钮高度
    static double buttonGap     = 0.10; // 按钮间距
    static double side          = 0.08; // 对话框外边界宽度

    static double bottomGap     = 0.15; // 按钮到对话框底部距离
    static double sideGap       = 0.15; // 按钮或文字到对话框两边距离

    // 分别算出按钮和文字需占用的宽度，其较大者决定对话框宽度
    double widthOfButtons = n*buttonWidth + (n-1)*buttonGap;
    double widthOfPrompts = TextStringWidth(prompt);

    // 对话框宽度
    double boxWidth  = 2*sideGap + Max(widthOfButtons, widthOfPrompts);
    double boxHeight;

    // 如果有文本框，则对话框高度需更大
    if (hasTextBox)
        boxHeight = 1.30;
    else
        boxHeight = 1.00;

    // 对话框绘制起点（居中显示）
    double box_X   = (winwidth  - boxWidth)  / 2;
    double box_Y   = (winheight - boxHeight) / 2;

    // 提示文字的绘制起点（居中）
    double labelX  = (winwidth - widthOfPrompts) / 2;
    double labelY  = box_Y + boxHeight - 2*bottomGap;

    // 按钮绘制起点（居中）
    double buttonX = (winwidth - widthOfButtons) / 2;
    double buttonY = box_Y + bottomGap;

    // 绘制外框架和内框架
    SetPenColor("Gray");
    drawRectangle(box_X-side, box_Y-side, boxWidth+2*side, boxHeight+2*side, FILL);
    SetPenColor("Light Gray");
    drawRectangle(box_X, box_Y, boxWidth, boxHeight, FILL);

    // 显示提示文字
    SetPenColor("White");
    MovePen(labelX, labelY);
    DrawTextString(prompt);

    // 如有文本框，则绘制
    if (hasTextBox)
    {
        double textBoxWidth  = 1.10;
        double textBoxHeight = 1.5*GetFontHeight();
        double textbox_X = (winwidth - textBoxWidth) / 2;
        double textbox_Y = buttonY + 4*buttonGap;

        textbox(GenUIID(0), textbox_X, textbox_Y, textBoxWidth, textBoxHeight,
            strBuffer, buflen);
    }

    // 绘制所有按钮
    int i, k, clicked = -1;
    for (i = 0; i < n; ++i)
    {
        k = button(GenUIID(i), buttonX + i*(buttonWidth+buttonGap), buttonY,
                    buttonWidth, buttonHeight, buttons[i]);

        if (k != 0) clicked = i;
    }

    return clicked;
}


/*
 * 函数名：MsgBoxResume
 * -------------
 * 提示游戏暂停状态
              -------------------
            |                   |
            |    Game paused    |
            |                   |
            |   -------------   |
            |  |   Resume    |  |
            |   -------------   |
             -------------------
 */
void MsgBoxResume()
{
    static char* prompt    = "Game paused";
    static char* buttons[] = {"Resume"};

    if (MsgBox(prompt, buttons, 1, NoTextBox, NULL, 0) == 0)
        gameResume();
}


/*
 * 函数名：MsgBoxResume
 * -------------
 * 提示保存游戏分数记录
              -----------------------------
            |                             |
            |      Enter your name:       |
            |        ------------         |
            |       |            |        |
            |        ------------         |
            |    --------     --------    |
            |   |   Ok   |   | Cancel |   |
            |    --------     --------    |
             -----------------------------
 */
void MsgBoxSave()
{
    static char* prompt    = "Enter your name:";
    static char* buttons[] = {"Ok", "Cancel"};
    static char  username[12];

    int choice = MsgBox(prompt, buttons, 2, HasTextBox, username, NAMELEN);

    if (choice == 0 && username[0] != '\0')
    {
        if (SUCCESS == saveGame(username))
            setGameStatu(ON_SUCCESS);
        else
            setGameStatu(ON_SAVEFAIL);
    }
    else if (choice == 1)
    {
        gameResume();
    }
}


/*
 * 函数名：MsgBoxLoad
 * -------------
 * 提示输入游戏存档名，用于读取存档
              -----------------------------
            |                             |
            |      Enter your name:       |
            |        ------------         |
            |       |            |        |
            |        ------------         |
            |    --------     --------    |
            |   |   Ok   |   | Cancel |   |
            |    --------     --------    |
             -----------------------------
 */
void MsgBoxLoad()
{
    static char* prompt    = "Enter your name:";
    static char* buttons[] = {"Ok", "Cancel"};
    static char  username[12];

    int choice = MsgBox(prompt, buttons, 2, HasTextBox, username, NAMELEN);

    if (choice == 0 && username[0] != '\0')
    {
        if (SUCCESS == loadGame(username))
        {
            initTetris();
            setGameStatu(ON_SUCCESS);
        }
        else
        {
            sprintf(errName, "%s", username);
            setGameStatu(ON_LOADFAIL);
        }
    }
    else if (choice == 1)
    {
        setGameStatu(ON_MAIN);
    }
}


/*
 * 函数名：MsgBoxSuccess
 * --------------
 * 提示“操作成功”
             -------------------------------
            |                               |
            |    Operation successfully.    |
            |                               |
            |          ----------           |
            |         |    Ok    |          |
            |          ----------           |
             -------------------------------
 */
void MsgBoxSuccess()
{
    static char* prompt    = "Operation successfully.";
    static char* buttons[] = {"Ok"};

    if (MsgBox(prompt, buttons, 1, NoTextBox, NULL, 0) == 0)
    {
        if (getPrevStatu() == ON_RECORD)
            setGameStatu(ON_MAIN);
        else
            gameResume();
    }
}


/*
 * 函数名：MsgBoxLoadFail
 * --------------
 * 提示读取存档失败
             --------------------------------------------------
            |                                                  |
            |    Record "XXX" doesn't exist or got damaged.    |
            |                                                  |
            |                  ----------                      |
            |                 |    Ok    |                     |
            |                  ----------                      |
             --------------------------------------------------
 */
void MsgBoxLoadFail()
{
    static char  prompt[50];
    static char* buttons[] = {"Ok"};

    sprintf(prompt, "Record \"%s\" doesn't exist or got damaged.", errName);

    if (MsgBox(prompt, buttons, 1, NoTextBox, NULL, 0) == 0)
    {
        setGameStatu(ON_LOAD);
    }
}


/*
 * 函数名：MsgBoxSaveFail
 * --------------
 * 提示保存（分数记录或存档）失败
 *（应该几乎用不到，除非遇到磁盘写入权限等问题）
             ---------------------------------
            |                                 |
            |    Failed to save game data.    |
            |                                 |
            |           ----------            |
            |          |    Ok    |           |
            |           ----------            |
             ---------------------------------
 */
void MsgBoxSaveFail()
{
    static char* prompt    = "Failed to save game data.";
    static char* buttons[] = {"Ok"};

    if (MsgBox(prompt, buttons, 1, NoTextBox, NULL, 0) == 0)
    {
        setGameStatu(ON_MAIN);
    }
}


/*
 * 函数名：MsgBoxRestart
 * --------------
 * 提示是否重新开始
             -----------------------------------------
            |                                         |
            |      Discard to start a new game?       |
            |                                         |
            |         --------     --------           |
            |        |   Ok   |   | Cancel |          |
            |         --------     --------           |
             -----------------------------------------
 */
void MsgBoxRestart()
{
    static char* prompt    = "Discard to start a new game?";
    static char* buttons[] = {"Ok", "Cancel"};

    int choice = MsgBox(prompt, buttons, 2, NoTextBox, NULL, 0);

    if (choice == 0)
        gameStart();
    else if (choice == 1)
        gameResume();
}


/*
 * 函数名：MsgBoxBackToMain
 * --------------
 * 提示是否回到主菜单
             -------------------------------------
            |                                     |
            |      Back to the main window?       |
            |                                     |
            |       --------     --------         |
            |      |   Ok   |   | Cancel |        |
            |       --------     --------         |
             -------------------------------------
 */
void MsgBoxBackToMain()
{
    static char* prompt    = "Back to the main window?";
    static char* buttons[] = {"Ok", "Cancel"};

    int choice = MsgBox(prompt, buttons, 2, NoTextBox, NULL, 0);

    if (choice == 0)
        setGameStatu(ON_MAIN);
    else if (choice == 1)
        gameResume();
}


/*
 * 函数名：MsgBoxGameOver
 * --------------
 * 提示总得分，及是否保存记录（游戏失败时使用）
             --------------------------------------------
            |                                            |
            |      You got XXX points! Save record?      |
            |                                            |
            |           --------     --------            |
            |          |   Ok   |   | Cancel |           |
            |           --------     --------            |
             --------------------------------------------
 */
void MsgBoxGameOver()
{
    static char  prompt[50];
    static char* buttons[] = {"Ok", "Cancel"};

    sprintf(prompt, "You got %d points! Save record?", getScore());

    int choice = MsgBox(prompt, buttons, 2, NoTextBox, NULL, 0);

    if (choice == 0)
    {
        setGameStatu(ON_RECORD);
    }
    else if (choice == 1)
    {
        setGameStatu(ON_MAIN);
    }
}


/*
 * 函数名：MsgBoxRecord
 * --------------
 * 提示输入保存分数时的用户名
             -----------------------------
            |                             |
            |      Enter your name:       |
            |        ------------         |
            |       |            |        |
            |        ------------         |
            |    --------     --------    |
            |   |   Ok   |   | Cancel |   |
            |    --------     --------    |
             -----------------------------
 */
void MsgBoxRecord()
{
    static char* prompt    = "Enter your name:";
    static char* buttons[] = {"Ok", "Cancel"};
    static char  strBuffer[12];

    int choice = MsgBox(prompt, buttons, 2, HasTextBox, strBuffer, NAMELEN);

    if (choice == 0 && strBuffer[0] != '\0')
    {
        if (SUCCESS == saveRecord(strBuffer, getScore()))
            setGameStatu(ON_SUCCESS);
        else
            setGameStatu(ON_SAVEFAIL);
    }
    else if (choice == 1)
    {
        setGameStatu(ON_MAIN);
    }
}