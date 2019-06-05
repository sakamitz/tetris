/*
 * 项目：Tetris
 * 文件名：tetris.c
 * 概览：方块操作模块
 * -------------
 * 负责功能：
 *      方块移动、旋转
 *      碰撞检测、消除检测
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "layout.h" // 需要取得方块堆大小
#include "tetris.h" // 本模块

/* 引入流程控制模块 flow.c 中的变量，分别为
 *      已落下的方块堆 stored
 *      正在下落的方块 falling
 *      下一个下落方块 nextBlock
 */
extern tetBoard stored[MaxCoordY+1][MaxCoordX+1];
extern tetBlock falling;
extern tetBlock nextBlock;

/* 坐标操作相关宏定义 */

// 将坐标变量 v 设为给定坐标 (nx, ny)
#define SetCoord(v, nx, ny) \
        { \
            (v).x = nx; \
            (v).y = ny; \
        }

// 绕中心格顺时针旋转一个周围格，即复数乘法 (x, y) * (-i) = (y, -x)
#define FlipCoord(v) \
        { \
            tmp = -(v).x; \
            (v).x = (v).y; \
            (v).y = tmp; \
        }

// 绕中心格逆时针旋转一个周围格，即复数乘法 (x, y) * i = (-y, x)
#define BackFlipCoord(v) \
        { \
            tmp = (v).x; \
            (v).x = -(v).y; \
            (v).y = tmp; \
        }

// 检测坐标 v 所在块是否被占用
#define AlreadyTaken(v) ( \
        InBound(v) && \
        stored[(v).y][(v).x].statu == CELL_TAKEN)


// 内部函数声明
void addToBoard();
int  eliminateLines();

/*
 * 函数名：initBlock
 * -------------
 * 将传入方块设为初始状态
 *
 * 有关方块的结构，定义一个中心格和三个周围格
 * 中心格储存绝对坐标，周围格储存相对中心格的坐标
 *
 * 这样定义的好处：
 * 一、节省空间；
 * 二、通用性好，所有方块可用一种结构表达；
 * 三、操作方便，无需考虑当前方向，移动只需改变中心格坐标，旋转只需做一个复数乘法
 *
 * 有关方块结构的图文说明以及初始状态的约定，详见：
 * https://www.processon.com/view/link/5cdf7a55e4b005286487410e
 */
void initBlock(tetBlock* b)
{
    // 根据方块类型设定初始状态
    switch (b->type)
    {
        case TET_I:
            SetCoord(b->surround[0],  0, -1);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  0,  2);
            break;
        case TET_J:
            SetCoord(b->surround[0], -1,  0);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  0,  2);
            break;
        case TET_L:
            SetCoord(b->surround[0],  1,  0);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  0,  2);
            break;
        case TET_O:
            SetCoord(b->surround[0],  1,  0);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  1,  1);
            break;
        case TET_S:
            SetCoord(b->surround[0], -1,  0);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  1,  1);
            break;
        case TET_Z:
            SetCoord(b->surround[0], -1,  1);
            SetCoord(b->surround[1],  0,  1);
            SetCoord(b->surround[2],  1,  0);
            break;
        case TET_T:
            SetCoord(b->surround[0],  0,  1);
            SetCoord(b->surround[1], -1,  0);
            SetCoord(b->surround[2],  1,  0);
            break;
        default:
            break;
    }
}

/*
 * 函数名：geneNext
 * -------------
 * 生成下一个方块
 */
void geneNext()
{
    // 随机选择方块类型和颜色
    nextBlock.type    = rand() % typeNum;
    nextBlock.color   = rand() % colorNum;

    // 将方块移动到右上角显示下一个的方框内
    moveToNextBox(&nextBlock);
    // 初始化方块
    initBlock(&nextBlock);
}

/*
 * 函数名：rotate
 * -------------
 * 顺时针旋转正在下落的方块
 */
void rotate()
{
    // O型方块无需旋转
    if (falling.type == TET_O)
        return;

    // 中心格不变，对周围格做复数乘法（乘以-i）
    int i, tmp;
    for (i = 0; i < 3; ++i)
    {
        FlipCoord(falling.surround[i]);
    }

    tetBoundStatu bs = collisionCheck();
    // 检测当前状态是否会产生碰撞
    switch (bs)
    {
        // 与底部或其他方块碰撞，则恢复原状态
        case COLLIDED:
            for (i = 0; i < 3; ++i)
            {
                BackFlipCoord(falling.surround[i]);
            }
            break;

        // 与左边界碰撞，则右移直到不再碰撞；或无法右移，恢复原状态
        case LEFT_BOUND:

            do {
                falling.coord.x++;
                bs = collisionCheck();
            } while (bs == LEFT_BOUND);

            if (bs == COLLIDED)
            {
                falling.coord.x--;
                for (i = 0; i < 3; ++i)
                    BackFlipCoord(falling.surround[i]);
            }
            break;

        // 与右边界碰撞，则左移直到不再碰撞；或无法左移，恢复原状态
        case RIGHT_BOUND:

            do {
                falling.coord.x--;
                bs = collisionCheck();
            } while (bs == RIGHT_BOUND);

            if (bs == COLLIDED)
            {
                falling.coord.x++;
                for (i = 0; i < 3; ++i)
                    BackFlipCoord(falling.surround[i]);
            }
            break;

        default:
            break;
    }
}

/*
 * 函数名：move
 * -------------
 * 移动正在下落的方块（左、右、下）
 */
void move(tetMove direction)
{
    // 下落
    if (direction == TM_DOWN)
    {
        falling.coord.y--;
        if (collisionCheck() == COLLIDED)
        {
            // 如碰撞，则添加到已落下方块，开始下一轮
            falling.coord.y++;
            addToBoard();
            newRound();
        }
    }
    // 左右移动
    else
    {
        falling.coord.x += direction;
        // 到达边界，则退回
        if (collisionCheck() != FREEMOVE)
            falling.coord.x -= direction;
    }
}

/*
 * 函数名：drop
 * -------------
 * 将正在下落的方块直接降落到底部，并开启下一轮
 */
void drop()
{
    // 不断下落，直到发生碰撞
    do {
        falling.coord.y--;
    } while (collisionCheck() == FREEMOVE);

    falling.coord.y++;
    // 添加到已落下方块，开始下一轮
    addToBoard();
    newRound();
}


/* （内部函数）
 * 函数名称：addToBoard
 * 函数原型：void addToBoard()
 * 功能描述：将正在下落的方块加入到已落下的方块堆中
 *        （在检测到碰撞或触底时使用）
 * 副作用？：改变全局变量stored
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：addToBoard();
 */
void addToBoard()
{
    int x = falling.coord.x;
    int y = falling.coord.y;

    // 将中心格加入堆中
    stored[y][x].statu = CELL_TAKEN;
    stored[y][x].color = falling.color;

    // 将3个周围格加入堆中
    int i;
    for (i = 0; i < 3; ++i)
    {
        // 切换到其中一个周围格上
        ShiftCoord(falling.coord, falling.surround[i]);
        x = falling.coord.x;
        y = falling.coord.y;
        // 将该块加入到堆中
        stored[y][x].statu = CELL_TAKEN;
        stored[y][x].color = falling.color;
        // 切换回中心格
        UnshiftCoord(falling.coord, falling.surround[i]);
    }

    // 消除检测
    eliminateLines();
}

/*
 * 函数名：collisionCheck
 * -------------
 * 碰撞检测
 * 返回值类型：枚举类型 tetBoundStatu，其定义见tetris.h
 * 可能取值：
 *      FREEMOVE    - 未碰撞
 *      COLLIDED    - 与堆中方块或界面底部碰撞
 *      LEFT_BOUND  - 与左边界碰撞
 *      RIGHT_BOUND - 与右边界碰撞
 */
tetBoundStatu collisionCheck()
{
    // 检测中心格
    if (falling.coord.x < 0)
        return LEFT_BOUND;
    else if (falling.coord.x > MaxCoordX)
        return RIGHT_BOUND;
    else if (AlreadyTaken(falling.coord) || falling.coord.y < 0)
        return COLLIDED;

    // 检测3个周围格
    int i;
    for (i = 0; i < 3; ++i)
    {
        // 切换到其中一个周围格上
        ShiftCoord(falling.coord, falling.surround[i]);

        if (falling.coord.x < 0)
        {
            // 切换回中心格并返回
            UnshiftCoord(falling.coord, falling.surround[i]);
            return LEFT_BOUND;
        }
        else if (falling.coord.x > MaxCoordX)
        {
            UnshiftCoord(falling.coord, falling.surround[i]);
            return RIGHT_BOUND;
        }
        else if (AlreadyTaken(falling.coord) || falling.coord.y < 0)
        {
            UnshiftCoord(falling.coord, falling.surround[i]);
            return COLLIDED;
        }

        // 切换回中心格
        UnshiftCoord(falling.coord, falling.surround[i]);
    }
    return FREEMOVE;
}

/* （内部函数）
 * 函数名称：eliminateLines
 * 函数原型：int eliminateLines()
 * 功能描述：消除检测，并作出处理
 * 副作用？：改变全局变量stored
 *
 * 参数描述：无
 * 返回类型：int，表示消除的行数
 * --------------
 * 使用方法：int eliminated = eliminateLines();
 */
int eliminateLines()
{
    int x, y;
    int eliminated = 0;
    bool lineFilled;

    // 从上到下按行扫描
    // 相比从下到上，在消除多行时运算量更小
    // 利用数组的行在内存中连续的特点
    // 用指针减少索引的开销
    tetBoard* curp;
    for (y = MaxCoordY; y >= 0; --y)
    {
        lineFilled = true;
        curp = &stored[y][0]; // 指向当前行开头
        for (x = 0; x <= MaxCoordX; ++x)
        {
            if (curp->statu == CELL_EMPTY)
            {
                lineFilled = false;
                break;
            }
            curp++; // 指向下一个方格
        }

        // 当前行已满，则消除
        if (lineFilled == true)
        {
            int yy;
            for (yy = y; yy < MaxCoordY; ++yy)
            {
                memcpy(&stored[yy][0], &stored[yy+1][0], sizeof(stored[0]));
            }
            eliminated++;
        }
    }

    // 分数增加
    addScore(eliminated);
}

/*
 * 函数名：moveToTop
 * -------------
 * 将指针指向的方块移动到屏幕上方
 */
void moveToTop(tetBlock* b)
{
    SetCoord(b->coord, MaxCoordX / 2, MaxCoordY);
}

/*
 * 函数名：moveToNextBox
 * -------------
 * 将指针指向的方块移动到显示下一个方块的盒内
 */
void moveToNextBox(tetBlock* b)
{
    SetCoord(b->coord, NextBlockX, NextBlockY);
}

/*
 * 函数名：moveToHoldBox
 * -------------
 * 将指针指向的方块移动到显示暂存方块的盒内
 */
void moveToHoldBox(tetBlock* b)
{
    SetCoord(b->coord, HoldBlockX, HoldBlockY);
}