/*
 * 项目：Tetris
 * 文件名：flow.h
 * 概览：流程控制模块
 * -------------
 * 主要内容：
 *      游戏状态、流程及相关操作接口的定义
 *
 * 外部接口：
 *      游戏流程初始化：
 *           initTetris
 *      游戏状态的读取与改变函数：
 *           setGameStatu
 *           getGameStatu
 *           getPrevStatu
 *           restoreGameStatu
 *           isOnHolding
 *      游戏数据的读取与改变函数
 *           addScore
 *           getScore
 *           getLevel
 *           getLines
 *      游戏进程控制函数
 *           newRound
 *           hold
 *           gameStart
 *           gamePause
 *           gameResume
 */

#ifndef FLOW_H
#define FLOW_H

#include <stdbool.h>

// 当前游戏状态
// 前4个分别代表：游戏暂停、游戏中、在主菜单界面、在排行榜界面
// 其余依此类推
typedef enum
{
  ON_PAUSE, ON_PLAYING, ON_MAIN, ON_RANKING, ON_HELP,
  ON_LOAD, ON_SAVE, ON_SUCCESS, ON_LOADFAIL, ON_SAVEFAIL,
  ON_RESTART, ON_BACKTOMAIN, ON_GAMEOVER, ON_RECORD
} tetGameStatus;


/*
 * 函数名称：initTetris
 * 函数原型：void initTetris()
 * 功能描述：初始化游戏控制机制（随机数种子及计时器回调事件）
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：initTetris();
 */
void initTetris();


/*
 * 函数名称：setGameStatu
 * 函数原型：void setGameStatu(tetGameStatus gs)
 * 功能描述：将当前游戏状态设为指定值，并更新显示
 * 副作用？：引起游戏状态改变
 *
 * 参数描述：待设置游戏状态 gs :: tetGameStatus
          （枚举类型tetGameStatus定义见本文件）
 * 返回类型：无
 * --------------
 * 使用方法：setGameStatu(yourGameStatu);
 */
void setGameStatu(tetGameStatus gs);


/*
 * 函数名称：getGameStatu
 * 函数原型：tetGameStatus getGameStatu()
 * 功能描述：取得当前游戏状态
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：tetGameStatus，表示当前游戏状态
          （枚举类型tetGameStatus定义见本文件）
 * --------------
 * 使用方法：tetGameStatus gs = getGameStatu();
 */
tetGameStatus getGameStatu();


/*
 * 函数名称：getPrevStatu
 * 函数原型：tetGameStatus getPrevStatu()
 * 功能描述：取得上一个游戏状态
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：tetGameStatus，表示当前游戏状态
          （枚举类型tetGameStatus定义见本文件）
 * --------------
 * 使用方法：tetGameStatus gs = getPrevStatu();
 */
tetGameStatus getPrevStatu();


/*
 * 函数名称：restoreGameStatu
 * 函数原型：void restoreGameStatu()
 * 功能描述：恢复上一个游戏状态，并更新显示
 * 副作用？：引起游戏状态改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：restoreGameStatu();
 */
void restoreGameStatu();


/*
 * 函数名称：isOnHolding
 * 函数原型：bool isOnHolding()
 * 功能描述：判断当前是否有方块暂存
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：bool，表示当前是否有方块暂存
 * --------------
 * 使用方法：bool whatever = isOnHolding();
 */
bool isOnHolding();


/*
 * 函数名称：newRound
 * 函数原型：void newRound()
 * 功能描述：结束本轮，开始下一轮方块下落
 *         | 检测游戏是否失败，以及处理难度升级的情况
 *         | 无需调用者判断暂存与释放状态，该函数自动处理）
 * 副作用？：引起游戏进程改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：newRound();
 */
void newRound();


/*
 * 函数名称：hold
 * 函数原型：void hold()
 * 功能描述：处理方块暂存与释放功能
           | 无需调用者判断暂存状态，该函数自动处理）
 * 副作用？：引起游戏进程改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：hold();
 */
void hold();


/*
 * 函数名称：addScore
 * 函数原型：void addScore(int eliminated)
 * 功能描述：根据消除的行数来增加分数
           | 基准分数：每行10分
           | 额外分数：同时消除多行时，每多一行增加5分
 * 副作用？：引起游戏分数改变
 *
 * 参数描述：同时消除的行数 eliminated :: int
 * 返回类型：无
 * --------------
 * 使用方法：addScore(yourEliminatedLines);
 */
void addScore(int eliminated);


/*
 * 函数名称：getScore
 * 函数原型：int getScore()
 * 功能描述：取得当前游戏分数
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：int，代表当前分数
 * --------------
 * 使用方法：int whatever = getScore();
 */
int getScore();


/*
 * 函数名称：getLevel
 * 函数原型：int getLevel()
 * 功能描述：取得当前游戏难度等级
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：int，代表当前游戏难度等级
 * --------------
 * 使用方法：int whatever = getLevel();
 */
int getLevel();


/*
 * 函数名称：getLines
 * 函数原型：int getLines()
 * 功能描述：取得当前已消除行数
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：int，代表当前已消除行数
 * --------------
 * 使用方法：int whatever = getLines();
 */
int getLines();


/*
 * 函数名称：gameStart
 * 函数原型：void gameStart()
 * 功能描述：开始一轮全新的游戏
 * 副作用？：引起游戏状态及游戏数据改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：gameStart();
 */
void gameStart();


/*
 * 函数名称：gamePause
 * 函数原型：void gamePause()
 * 功能描述：暂停游戏
 * 副作用？：引起游戏状态改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：gamePause();
 */
void gamePause();


/*
 * 函数名称：gameResume
 * 函数原型：void gameResume()
 * 功能描述：继续游戏
 * 副作用？：引起游戏状态改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：gameResume();
 */
void gameResume();

#endif