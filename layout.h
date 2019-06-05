/*
 * 项目：Tetris
 * 文件名：layout.h
 * 概览：界面绘制模块
 * -------------
 * 主要内容：界面位置、大小相关的数据和外部接口定义
 * 外部接口：
 *      游戏界面：框架、方块盒、分数栏
 *          dispFrame
 *          dispStat
 *      游戏菜单、标题栏
 *          dispGameMenu
 *          dispHeader
 *      各个窗口：主界面、排行榜、帮助界面
 *          dispStartWindow
 *          dispRankWindow
 *          dispHelpWindow
 *      所有的方块绘制
 *          dispAllBlocks
 *
 * （该模块的外部接口一般只在main模块中的界面更新函数中使用）
 */

#ifndef LAYOUT_H
#define LAYOUT_H

// 下一个方块、暂存方块的坐标
// 单位：自建坐标 (0, 0) ~ (9, 20)
#define NextBlockX  13
#define NextBlockY  18
#define HoldBlockX  13
#define HoldBlockY  11

#define FILL     1
#define NOFILL   0

// 最大坐标值 x = 9, y = 20
// 自建坐标范围 (0, 0) ~ (9, 20)
#define MaxCoordX   9
#define MaxCoordY   20


// 检测坐标 v 是否在绘图区域中
#define InBound(v) ( \
        (v).x <= MaxCoordX && \
        (v).y <= MaxCoordY && \
        (v).x >= 0 && \
        (v).y >= 0)

// 屏幕坐标定义
typedef struct {
  double x;
  double y;
} tetInch;

// 控制键定义，用于帮助界面
typedef enum
{
  NullKey, W, S, A, D, E, F
} controlKeys;


/*
 * 函数名称：dispFrame
 * 函数原型：void dispFrame()
 * 功能描述：绘制游戏 board 框架和两个方块盒
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispFrame();
 */
void dispFrame();


/*
 * 函数名称：dispStat
 * 函数原型：void dispStat()
 * 功能描述：显示游戏数据：分数、难度等级、消除行数
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispStat();
 */
void dispStat();


/*
 * 函数名称：dispGameMenu
 * 函数原型：void dispGameMenu()
 * 功能描述：显示上方游戏菜单栏
 * 副作用？：界面绘制、游戏流程改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispGameMenu();
 */
void dispGameMenu();


/*
 * 函数名称：dispHeader
 * 函数原型：void dispHeader(char* msg)
 * 功能描述：在上方标题栏显示一行提示文字
 * 副作用？：界面绘制
 *
 * 参数描述：msg :: char*，代表显示文字
 * 返回类型：无
 * --------------
 * 使用方法：dispHeader(textYouWantToShow);
 */
void dispHeader(char* msg);


/*
 * 函数名称：dispStartWindow
 * 函数原型：void dispStartWindow()
 * 功能描述：绘制开始界面，包含4个按钮：
 *          开始、载入、排行榜、帮助
 * 副作用？：界面绘制、游戏流程改变
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispStartWindow();
 */
void dispStartWindow();


/*
 * 函数名称：dispRankWindow
 * 函数原型：void dispRankWindow()
 * 功能描述：绘制排行榜界面
 * 副作用？：界面绘制、磁盘读取
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispRankWindow();
 */
void dispRankWindow();


/*
 * 函数名称：dispHelpWindow
 * 函数原型：void dispHelpWindow()
 * 功能描述：绘制帮助界面
 *         | 含有开发人员、源码地址、操作帮助
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispHelpWindow();
 */
void dispHelpWindow();


/*
 * 函数名称：dispAllBlocks
 * 函数原型：void dispAllBlocks()
 * 功能描述：绘制所有方块，包括
 *         | 已落下方块堆
 *         | 正在下落方块
 *         | 即将下落方块
 *         | 用户暂存方块
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：dispAllBlocks();
 */
void dispAllBlocks();

#endif