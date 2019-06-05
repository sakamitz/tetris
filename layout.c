/*
 * 项目：Tetris
 * 文件名：layout.c
 * 概览：界面绘制模块
 * -------------
 * 负责功能：界面绘制
 *      游戏界面：显示框、方块盒、分数栏
 *      游戏菜单、标题栏
 *      各个窗口：主界面、排行榜、帮助界面
 *      所有的方块绘制
 */

#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"

#include "imgui.h"  // 需要绘制控件
#include "fileIO.h" // 排行榜需要调用文件读取
#include "tetris.h" // 需要进行方块绘制
#include "flow.h"   // 需要在点击时改变或取得游戏状态
#include "layout.h" // 本模块

// 界面位置定义
#define leftFrmX    0.07
#define leftFrmY    0.15
#define rightFrmX   3.40
#define rightFrmY   5.15
#define statX       3.40
#define statY       2.60
// 方块边长为 0.30
#define cellSide    0.30


tetRecord* records;

// 引入main模块中的表示窗口宽度及高度的变量
extern double winwidth, winheight;

// 引入流程控制模块 flow.c 中定义的方块，用于绘制
extern tetBoard stored[MaxCoordY+1][MaxCoordX+1];
extern tetBlock falling;
extern tetBlock nextBlock;
extern tetBlock holdBlock;


/*
 * 函数名：dispFrame
 * -------------
 * 绘制游戏 board 框架和两个方块盒
 */
void dispFrame()
{
    static double frmBoardWidth  = 3.10;
    static double frmBoardHeight = 6.50;
    static double frmNextWidth   = 1.50;
    static double frmNextHeight  = 1.50;
    static double frmHoldWidth   = 1.50;
    static double frmHoldHeight  = 1.50;

    SetPenColor("White");
    drawRectangle(leftFrmX, leftFrmY,
        frmBoardWidth, frmBoardHeight, NOFILL);

    drawRectangle(rightFrmX, rightFrmY,
        frmNextWidth,  frmNextHeight,  NOFILL);

    drawRectangle(rightFrmX, rightFrmY - frmHoldHeight - 0.45,
        frmHoldWidth,  frmHoldHeight,  NOFILL);
}


/*
 * 函数名：dispStat
 * -------------
 * 显示游戏数据：分数、难度等级、消除行数
 */
void dispStat()
{
    static char strBuffer[10];
    double h = 1.3*GetFontHeight();

    SetPenColor("White");
    drawLabel(statX, statY,     "Score");
    drawLabel(statX, statY-h,   itoa(getScore(), strBuffer, 10));
    drawLabel(statX, statY-2*h, "Level");
    drawLabel(statX, statY-3*h, itoa(getLevel(), strBuffer, 10));
    drawLabel(statX, statY-4*h, "Lines");
    drawLabel(statX, statY-5*h, itoa(getLines(), strBuffer, 10));

}

/*
 * 函数名：dispGameMenu
 * -------------
 * 显示上方游戏菜单栏
 */
void dispGameMenu()
{
    static char* mnuPause   = "Pause (P)";
    static char* mnuResume  = "Resume (P)";
    static char* mnuSave    = "Save (K)";
    static char* mnuRestart = "Restart (N)";
    static char* mnuMain    = "Main (M)";
    static char* mnuHelp    = "Help (L)";

    double h = 1.5*GetFontHeight();
    double w = winwidth/5;
    double x = 0;
    double y = winheight - h;

    // 绘制菜单栏
    if (getGameStatu() == ON_PLAYING)
    {
        if (menuItem(GenUIID(0), x, y, w, h, mnuPause))
            gamePause();
    }
    else
    {
        if (menuItem(GenUIID(0), x, y, w, h, mnuResume))
            gameResume();
    }

    if (menuItem(GenUIID(0), x+w, y, w, h, mnuSave))
    {
        gamePause();
        setGameStatu(ON_SAVE);
    }

    if (menuItem(GenUIID(0), x+2*w, y, w, h, mnuRestart))
    {
        gamePause();
        setGameStatu(ON_RESTART);
    }

    if (menuItem(GenUIID(0), x+3*w, y, w, h, mnuMain))
    {
        gamePause();
        setGameStatu(ON_BACKTOMAIN);
    }

    if (menuItem(GenUIID(0), x+4*w, y, w, h, mnuHelp))
    {
        gamePause();
        setGameStatu(ON_HELP);
    }
}

/*
 * 函数名：dispHeader
 * -------------
 * 在上方标题栏显示一行提示文字
 */
void dispHeader(char* msg)
{
    double h = 1.5*GetFontHeight();
    double w = winwidth;
    double x = 0;
    double y = winheight - h;

    menuItem(GenUIID(0), x, y, w, h, msg);
}

/*
 * 函数名：dispStartWindow
 * -------------
 * 绘制开始界面
 */
void dispStartWindow()
{
    static double frmStartWidth  = 2.00;
    static double frmStartHeight = 4.00;
    static double offset         = 0.10; // 用于绘制立体方框

    static double buttonWidth    = 1.20;
    static double buttonHeight   = 0.50;
    static double buttonGap      = 1.00;

    double frmStartX = (winwidth - frmStartWidth) / 2;
    double frmStartY = (winheight - frmStartHeight) / 2;
    double buttonX   = (winwidth - buttonWidth) / 2;
    double buttonY   = (winheight + 2.5*buttonGap) / 2;

    // 绘制大方框
    SetPenColor("Gray");
    drawRectangle(frmStartX+offset, frmStartY-offset,
        frmStartWidth, frmStartHeight, FILL);
    SetPenColor("Light Gray");
    drawRectangle(frmStartX, frmStartY,
        frmStartWidth, frmStartHeight, FILL);

    // 绘制4个按钮
    bool clicked = false;
    if (button(GenUIID(0), buttonX, buttonY,
        buttonWidth, buttonHeight, "New Game"))
    {
        clicked = true;
        gameStart();
    }

    if (button(GenUIID(0), buttonX, buttonY - buttonGap,
        buttonWidth, buttonHeight, "Load Game"))
    {
        clicked = true;
        setGameStatu(ON_LOAD);
    }

    if (button(GenUIID(0), buttonX, buttonY - 2*buttonGap,
        buttonWidth, buttonHeight, "Ranking"))
    {
        clicked = true;
        // 点击排行榜按钮，则读取记录数据
        if (records != NULL)
            freeRecords(records);
        records = loadRecords();
        setGameStatu(ON_RANKING);
    }

    if (button(GenUIID(0), buttonX, buttonY - 3*buttonGap,
        buttonWidth, buttonHeight, "Help"))
    {
        clicked = true;
        setGameStatu(ON_HELP);
    }

    // 如果点击了按钮则刷新界面
    if (clicked) display();
}

/*
 * 函数名：dispRankWindow
 * -------------
 * 绘制排行榜界面
 */
void dispRankWindow()
{
    static char* title     = "Ranking List";
    static char* headers[] = {"Ranking", "Player", "Score"};

    static double frmRankWidth  = 4.20;
    static double frmRankHeight = 6.00;

    static double buttonBackWidth   = 0.55;
    static double buttonBackHeight  = 0.30;

    double frmRankX = (winwidth  - frmRankWidth) / 2;
    double frmRankY = (winheight - frmRankHeight) / 2;

    double labelX    = frmRankX + 0.20;
    double labelY    = frmRankY + frmRankHeight - 0.80;
    double labelGap  = 2.4*GetFontHeight();
    double columnGap = 35*TextStringWidth(" ");

    double buttonBackX  = (winwidth - buttonBackWidth) / 2;
    double buttonBackY  = frmRankY + 0.20;

    SetPointSize(20);
    double titleX   = (winwidth - TextStringWidth(title)) / 2;
    double titleY   = labelY + labelGap;

    // 绘制大方框
    SetPenColor("Gray");
    drawRectangle(frmRankX, frmRankY, frmRankWidth, frmRankHeight, FILL);

    // 显示标题
    SetPenColor("White");
    MovePen(titleX, titleY);
    DrawTextString(title);
    SetPointSize(10);

    // 显示3栏
    int i;
    SetPenColor("Black");
    for (i = 0; i < 3; ++i)
    {
        MovePen(labelX + i*columnGap, labelY);
        DrawTextString(headers[i]);
    }

    // 显示玩家名、分数数据
    char labelMsg[10];
    char strScore[10];
    tetRecord *rec = records;
    // 前三名颜色高亮
    SetPenColor("Green");
    for (i = 1; i <= 10; ++i)
    {
        if (i > 3) SetPenColor("White");

        sprintf(labelMsg, "NO.%d", i);
        MovePen(labelX, labelY - i*labelGap);
        DrawTextString(labelMsg);

        // 即使数据不足10个，也要显示第一栏
        if (rec != NULL)
        {
            MovePen(labelX + columnGap, labelY - i*labelGap);
            DrawTextString(rec->name);

            MovePen(labelX + 2*columnGap, labelY - i*labelGap);
            DrawTextString(itoa(rec->score, strScore, 10));

            rec = rec->next;
        }
    }

    // 点击返回按钮
    if (button(GenUIID(0), buttonBackX, buttonBackY,
        buttonBackWidth, buttonBackHeight, "Back"))
    {
        restoreGameStatu();
        display();
    }
}

/*
 * 函数名：dispRankWindow
 * -------------
 * 绘制排行榜界面
 */
void dispHelpWindow()
{
    static char* title = "Tetris";
    static char* developers[] =
    {
        "By C Group 23 - (in alphabetical order)",
        "Shaolin Huang",
        "Fanjie Ruan",
        "Xiangnan Yang"
    };
    static char* helpMsg[] =
    {
        "Click the buttons to get help",
        "Rotate the falling tetrimino",
        "Drop down the tetrimino",
        "Move the tetrimino left",
        "Move the tetrimino right",
        "Hold/Release the falling tetrimino",
        "Drop faster"
    };

    static double frmHelpWidth  = 4.00;
    static double frmHelpHeight = 6.00;

    static double buttonWidth   = 0.70;
    static double buttonHeight  = 0.35;
    static double keyWidth      = 0.50;
    static double keyHeight     = 0.50;
    static double buttonGap     = 0.05;

    // 定义按下的按钮
    static controlKeys selected = NullKey;

    // 绘制大方框
    double frmHelpX = (winwidth - frmHelpWidth) / 2;
    double frmHelpY = (winheight - frmHelpHeight) / 2;
    SetPenColor("Gray");
    drawRectangle(frmHelpX, frmHelpY, frmHelpWidth, frmHelpHeight, FILL);

    // 大字号绘制标题"Tetris"
    SetPointSize(40);
    SetPenColor("Brown");
    double titleX = (winwidth - TextStringWidth(title)) / 2;
    double titleY = frmHelpY + frmHelpHeight - 1.2*GetFontHeight();
    MovePen(titleX, titleY);
    DrawTextString(title);

    // 显示开发者名单
    SetPointSize(10);
    SetPenColor("Dark Gray");
    double devLabelX;
    double devLabelY = titleY;
    int i;
    for (i = 0; i < 4; ++i)
    {
        devLabelX  = (winwidth - TextStringWidth(developers[i])) / 2;
        devLabelY -= 2 * GetFontHeight();
        MovePen(devLabelX, devLabelY);
        DrawTextString(developers[i]);
    }

    // 显示 "GitHub" 按钮
    double githubX = (winwidth - buttonWidth) / 2;
    double githubY = devLabelY - 3*GetFontHeight();
    if (button(GenUIID(0), githubX, githubY,
        buttonWidth, buttonHeight, "GitHub"))
    {
        ShellExecute(NULL, "open", "https://github.com/sakamitz/tetris",
            NULL, NULL, SW_MINIMIZE);
    }

    // 绘制6个控制按钮
    double keyX  = (winwidth - 2*keyWidth) / 2;
    double keyY  = githubY - 8*GetFontHeight();
    if (button(GenUIID(0), keyX, keyY,
        keyWidth, keyHeight, "S"))
    {
        selected = S;
    }

    if (button(GenUIID(0), keyX, keyY+keyHeight,
        keyWidth, keyHeight, "W"))
    {
        selected = W;
    }

    if (button(GenUIID(0), keyX-keyWidth, keyY,
        keyWidth, keyHeight, "A"))
    {
        selected = A;
    }

    if (button(GenUIID(0), keyX+keyWidth, keyY,
        keyWidth, keyHeight, "D"))
    {
        selected = D;
    }

    if (button(GenUIID(0),
        keyX + keyWidth + buttonGap,
        keyY + keyHeight + buttonGap,
        keyWidth, keyHeight, "E"))
    {
        selected = E;
    }

    if (button(GenUIID(0),
        keyX + 2*keyWidth + buttonGap, keyY,
        keyWidth, keyHeight, "F"))
    {
        selected = F;
    }

    // 根据按下的按钮，显示对应的帮助信息
    SetPenColor("Black");
    double helpLabelX = (winwidth - TextStringWidth(helpMsg[selected])) / 2;
    double helpLabelY = keyY - 3*GetFontHeight();
    MovePen(helpLabelX, helpLabelY);
    DrawTextString(helpMsg[selected]);

    // 显示“返回”按钮
    double buttonBackX = (winwidth - buttonWidth) / 2;
    double buttonBackY = frmHelpY + 0.20;
    if (button(GenUIID(0), buttonBackX, buttonBackY,
        buttonWidth, buttonHeight, "Back"))
    {
        restoreGameStatu();
        display();
    }
}

/* （内部函数）
 * 函数：coordToInch
 * 用法：tetInch inch = coordToInch(yourCoord);
 * ------------------------
 * 将自建坐标转换为屏幕像素坐标
 * 自建坐标范围：(0, 0) ~ (9, 20)
 */
tetInch coordToInch(tetCoord coord)
{
    tetInch inch;
    inch.x = coord.x * cellSide + leftFrmX + 0.05; // 与边界保持微小间距
    inch.y = coord.y * cellSide + leftFrmY + 0.09;

    return inch;
}

/* （内部函数）
 * 函数：fillCell
 * 用法：fillCell(yourCoord, yourColor);
 * ------------------------
 * 用指定颜色填充指定坐标的格子（自建坐标）
 * 自建坐标范围：(0, 0) ~ (9, 20)
 */
void fillCell(tetCoord coord, tetColor color)
{
    tetInch inch = coordToInch(coord);

    SetPenColor(colorMap[color]);
    drawRectangle(inch.x, inch.y, cellSide, cellSide, 1);
}

/* （内部函数）
 * 函数：drawBlock
 * 用法：drawBlock(&yourBlock);
 * ------------------------
 * 绘制一个方块
 * （因为方块结构的特殊，我们不必考虑方块的类型和方向）
 */
void drawBlock(tetBlock* b)
{
    // 绘制中心格
    fillCell(b->coord, b->color);

    // 绘制3个周围格
    int i;
    for (i = 0; i < 3; ++i)
    {
        ShiftCoord(b->coord, b->surround[i]);
        // 只有处于有效范围内的格子才绘制
        // 有效范围即 board 界面和两个方块盒
        if (b->coord.x > 10 || InBound(b->coord))
            fillCell(b->coord, b->color);
        UnshiftCoord(b->coord, b->surround[i]);
    }
}

/* （内部函数）
 * 函数：dispStored
 * 用法：dispStored();
 * ------------------------
 * 绘制已落下的方块堆
 */
void dispStored()
{
    int x, y;
    tetCoord coord;
    // 利用二维数组在内存中连续的特性
    // 使用指针遍历，能节省索引的开销
    tetBoard* curp = &stored[0][0];
    for (y = 0; y <= MaxCoordY; ++y)
    {
        for (x = 0; x <= MaxCoordX; ++x)
        {
            if (curp->statu == CELL_TAKEN)
            {
                coord.x = x;
                coord.y = y;
                fillCell(coord, curp->color);
            }
            curp++;
        }
    }
}

/* （内部函数）
 * 函数：dispFalling
 * 用法：dispFalling();
 * ------------------------
 * 绘制正在下落的方块
 */
void dispFalling()
{
    drawBlock(&falling);
}

/* （内部函数）
 * 函数：dispNext
 * 用法：dispNext();
 * ------------------------
 * 绘制下一个下落的方块
 */
void dispNext()
{
    drawBlock(&nextBlock);
}

/* （内部函数）
 * 函数：dispHold
 * 用法：dispHold();
 * ------------------------
 * 绘制暂存的方块
 */
void dispHold()
{
    if (isOnHolding())
    {
        drawBlock(&holdBlock);
    }
}

/*
 * 函数：dispAllBlocks
 * ------------------------
 * 绘制界面上所有的方块
 */
void dispAllBlocks()
{
    dispNext();
    dispHold();
    dispFalling();
    dispStored();
}