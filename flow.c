/*
 * 项目：Tetris
 * 文件名：flow.c
 * 概览：流程控制模块
 * -------------
 * 负责功能：流程控制，包括
 *      计时器的控制（下落速度）
 *      游戏数据更新（速度、分数等）
 *      游戏状态控制（暂停、继续、重来、结束等）
 *      游戏进程控制（方块暂存/释放、难度提升、开启下一轮等）
 */

#include <stdio.h>
#include <string.h>

#include "layout.h"  // 需要取得方块堆大小
#include "tetris.h"  // 需要取得方块结构定义
#include "flow.h"    // 本模块

// 下落计时器ID
#define Timer_Fall  1

int speed;  // 游戏速度
int score;  // 当前分数
int level;  // 当前难度等级，最高为5
int lines;  // 已消除的行数

// 已落下的方块堆
tetBoard stored[MaxCoordY+1][MaxCoordX+1];
tetBlock falling;    // 正在下落的方块
tetBlock nextBlock;  // 下一个下落方块
tetBlock holdBlock;  // 当前暂存的方块

bool OnHolding;      // 是否有暂存方块
bool OnRelease;      // 是否刚释放了暂存方块
bool delayOneRound;  // 释放暂存方块后需暂停一轮hold功能

// 刚进入游戏时，状态为打开主菜单
tetGameStatus GameStatu = ON_MAIN;
// 保存上一个游戏状态，以供"返回"功能的实现
tetGameStatus prevStatu;

// 内部函数声明
void levelUp();
void gameOver();
void TimerFallingEvent(int timerID);


/*
 * 函数名：initTetris
 * -------------
 * 初始化游戏控制机制
 */
void initTetris()
{
    // 初始化随机数种子，用于随机方块的产生
    srand((unsigned)time(NULL));
    // 注册计时器回调函数
    registerTimerEvent(TimerFallingEvent);
}

/*
 * 函数名：initGameData
 * -------------
 * 初始化游戏数据（分数、难度等级等）
 */
void initGameData()
{
    speed = 1000; // 初始降落速度为 1格/秒
    score = 0;
    level = 1;
    lines = 0;

    OnHolding     = false;
    OnRelease     = false;
    delayOneRound = false;

    // 将方块堆置为空
    memset(&stored[0][0], CELL_EMPTY, sizeof(stored));
}

/*
 * 函数名：TimerFallingEvent
 * -------------
 * 计时器回调函数，下降方块并更新显示
 */
void TimerFallingEvent(int timerID)
{
    move(TM_DOWN);
    display();
}

/*
 * 函数名：setGameStatu
 * -------------
 * 将当前游戏状态设为指定值，并更新显示
 * 参数类型：枚举类型 tetGameStatus，其定义见flow.h
 */
void setGameStatu(tetGameStatus gs)
{
    // 保存上一个状态
    prevStatu = GameStatu;
    GameStatu = gs;
    display();
}

/*
 * 函数名：getGameStatu
 * -------------
 * 取得当前游戏状态
 * 返回类型：枚举类型 tetGameStatus，其定义见flow.h
 */
tetGameStatus getGameStatu()
{
    return GameStatu;
}

/*
 * 函数名：getPrevStatu
 * -------------
 * 取得上一个游戏状态
 * 返回类型：枚举类型 tetGameStatus，其定义见flow.h
 */
tetGameStatus getPrevStatu()
{
    return prevStatu;
}

/*
 * 函数名：restoreGameStatu
 * -------------
 * 恢复上一个游戏状态，并更新显示
 */
void restoreGameStatu()
{
    GameStatu = prevStatu;
    display();
}

/*
 * 函数名：isOnHolding
 * -------------
 * 当前是否有方块暂存
 * 返回类型：bool
 */
bool isOnHolding()
{
    return OnHolding;
}

/*
 * 函数名：newRound
 * -------------
 * 开始下一轮方块下落，同时检测游戏是否失败，
 * 以及处理难度升级的情况
 */
void newRound()
{
    // 即将释放出暂存的方块
    if (OnRelease && delayOneRound)
    {
        falling = holdBlock;
        delayOneRound = false;
    }
    // 上一轮释放了暂存的方块，解禁hold功能
    else if (OnRelease)
    {
        falling = nextBlock;
        OnRelease = false;
        geneNext();
    }
    // 与hold无关的普通情况
    else
    {
        falling = nextBlock;
        geneNext();
    }

    // 将方块位置设置到顶端
    moveToTop(&falling);
    // 如果开头就发生碰撞，则游戏失败
    if (collisionCheck() == COLLIDED)
        gameOver();

    // 每消除10行，就增加1级难度，直到5级
    if (level <= lines / 10 && level < 5)
        levelUp();
}

/*
 * 函数名：hold
 * -------------
 * 处理方块暂存与释放
 */
void hold()
{
    // 如果刚释放方块，则该一轮暂时禁用hold功能
    // 避免玩家不断进行hold，从而随机到想要的下一个方块
    if (OnRelease)
        return;

    // 如果没有方块暂存，则将当前下落方块暂存
    if (OnHolding == false)
    {
        holdBlock = falling;
        moveToHoldBox(&holdBlock);
        initBlock(&holdBlock);
        OnRelease = false;
    }
    // 如果有方块暂存，则将其释放
    else
    {
        nextBlock = falling;
        moveToNextBox(&nextBlock);
        initBlock(&nextBlock);
        OnRelease = true;
        delayOneRound = true;
    }

    // 开始下一轮
    OnHolding = !OnHolding;
    newRound();
}

/* （内部函数）
 * 函数名：levelUp
 * -------------
 * 难度升级
 * 增大难度的方式：将计时器间隔缩小为四分之三，
 * 即当前下落速度变为上一级的1.33倍
 */
void levelUp()
{
    level += 1;
    // 将计时器间隔缩小为四分之三
    speed = speed / 4 * 3;
    /* 注意这个speed的含义，
     * 它其实是用来作为下落计时器的时间间隔参数的，
     * 而真正的速度应该和时间间隔成反比。
     * 但将其改为interval之类的名字，
     * 则其含义不如speed直观，
     * 也没必要引入另一个变量来表示时间间隔
     */
    startTimer(Timer_Fall, speed);
}

/*
 * 函数名：addScore
 * -------------
 * 根据同时消除的行数来增加分数
 * 参数类型：int，代表同时消除的行数
 */
void addScore(int eliminated)
{
    // 基准分数：每行10分
    score += 10 * eliminated;
    lines += eliminated;

    // 额外分数：同时消除多行时，每多一行增加5分
    if (eliminated > 1)
        score += 5 * (eliminated - 1);
}

/*
 * 函数名：getScore
 * -------------
 * 取得当前分数
 * 返回类型：int，表示分数
 */
int getScore()
{
    return score;
}

/*
 * 函数名：getLevel
 * -------------
 * 取得当前难度等级
 * 返回类型：int，表示难度等级
 */
int getLevel()
{
    return level;
}

/*
 * 函数名：getLines
 * -------------
 * 取得已消除行数
 * 返回类型：int，表示已消除行数
 */
int getLines()
{
    return lines;
}

/*
 * 函数名：gameStart
 * -------------
 * 开始一局新的游戏
 */
void gameStart()
{
    initTetris();
    initGameData();
    setGameStatu(ON_PLAYING);
    startTimer(Timer_Fall, speed);
    geneNext();
    newRound();
    display();
}

/*
 * 函数名：gamePause
 * -------------
 * 暂停游戏
 */
void gamePause()
{
    setGameStatu(ON_PAUSE);
    cancelTimer(Timer_Fall);
}

/*
 * 函数名：gameResume
 * -------------
 * 继续游戏
 */
void gameResume()
{
    setGameStatu(ON_PLAYING);
    startTimer(Timer_Fall, speed);
}

/* （内部函数）
 * 函数名：gameOver
 * -------------
 * 游戏失败
 */
void gameOver()
{
    setGameStatu(ON_GAMEOVER);
    cancelTimer(Timer_Fall);
}