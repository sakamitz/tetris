/*
 * 项目：Tetris
 * 文件名：tetris.h
 * 概览：方块操作模块
 * -------------
 * 主要内容：
 *      方块结构、外观、位置、运动相关的定义
 *
 * 外部接口：
 *      方块初始化函数：
            initBlock
            geneNext
 *      运动函数：
            rotate
            move
            drop
 *      碰撞检测函数：
            collisionCheck
 */

#ifndef TETRIS_H
#define TETRIS_H


/* 第一部分 -- 坐标操作相关宏定义 */

// 将当前操作坐标从 v 移到 w 上
#define ShiftCoord(v, w) \
        { \
            (v).x += (w).x; \
            (v).y += (w).y; \
        }

// 将当前操作坐标从 w 移回 v 上
#define UnshiftCoord(v, w) \
        { \
            (v).x -= (w).x; \
            (v).y -= (w).y; \
        }

/* 第二部分 -- 方块结构、外观、位置、运动相关数据结构定义 */

// 方块颜色表，共5种颜色
static char* colorMap[] =
{
    "Light Gray",
    "Red",
    "Yellow",
    "Green",
    "Cyan"
};

// 方块颜色定义，共5种颜色；colorNum 默认值为5，用于代表所有颜色数
typedef enum {
  LightGray, Red, Yellow, Green, Cyan, colorNum
} tetColor;


// 方块类型定义，共7种；typeNum 默认值为7，用于代表所有种类数
// 方块的样式可由字母形状得到提示，例如，"I"代表长条形方块
typedef enum {
  TET_I, TET_J, TET_L, TET_O, TET_S, TET_Z, TET_T, typeNum
} tetType;


// 方块三种可能的移动（左、右、下），设置初始值是为了坐标操作方便
typedef enum {
  TM_LEFT = -1, TM_RIGHT = 1, TM_DOWN
} tetMove;


// 游戏主界面格子的两种可能状态，为空或者被占用
typedef enum {
  CELL_EMPTY, CELL_TAKEN
} tetCellStatu;


/* 碰撞检测结果定义
 *      FREEMOVE    - 未碰撞
 *      COLLIDED    - 与堆中方块或界面底部碰撞
 *      LEFT_BOUND  - 与左边界碰撞
 *      RIGHT_BOUND - 与右边界碰撞
 */
typedef enum {
  FREEMOVE, COLLIDED, LEFT_BOUND, RIGHT_BOUND
} tetBoundStatu;


// 自建坐标定义，表示方块在游戏界面中的位置
// 取值为整数，范围 (0, 0) ~ (9, 20)
typedef struct {
  int x;
  int y;
} tetCoord;


/* 方块结构定义
 * 有关方块的结构，定义一个中心格和三个周围格
 * 中心格储存绝对坐标，周围格储存相对中心格的坐标
 *
 * 这样定义的好处：
 * 一、节省空间；
 * 二、通用性好，所有方块可用一种结构表达；
 * 三、操作方便，无需考虑当前方向，移动只需改变中心格坐标，旋转只需做一个复数乘法
 *
 * 有关方块结构的图文说明以及初始状态的约定，详见程序报告及用户手册，或：
 * https://www.processon.com/view/link/5cdf7a55e4b005286487410e
 *
 * 该结构中包含信息：方块类型、颜色、中心坐标、3个周围格
 * 3个周围格只需储存相对坐标，不需要类型、颜色等信息
 */
typedef struct {
  tetType  type;
  tetColor color;
  tetCoord coord;
  tetCoord surround[3];
} tetBlock;


// 已落下的方块堆定义
// 包含信息：当前格是否为空、当前格颜色
typedef struct {
  tetCellStatu statu;
  tetColor color;
} tetBoard;



/* 第二部分 -- 外部接口函数定义 */

/*
 * 函数名称：initBlock
 * 函数原型：void initBlock(tetBlock* b)
 * 功能描述：将方块设为初始状态
 * 副作用？：改变传入方块形态
 *
 * 参数描述：待操作方块地址 b :: tetBlock*
 * 返回类型：无
 * --------------
 * 使用方法：initBlock(&yourBlock);
 */
void initBlock(tetBlock* b);


/*
 * 函数名称：geneNext
 * 函数原型：void geneNext()
 * 功能描述：随机生成下一个方块，并将其移动到显示下一个方块的方框内
 * 副作用？：改变全局变量nextBlock
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：geneNext();
 */
void geneNext();


/*
 * 函数名称：rotate
 * 函数原型：void rotate()
 * 功能描述：顺时针旋转正在下落的方块
 * 副作用？：改变全局变量falling
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：geneNext();
 */
void rotate();


/*
 * 函数名称：move
 * 函数原型：void move(tetMove direction)
 * 功能描述：移动正在下落的方块（左、右、下）
 * 副作用？：改变全局变量falling
 *
 * 参数描述：方块移动方向 direction :: tetMove
 *        （枚举类型tetMove定义见本文件）
 * 返回类型：无
 * --------------
 * 使用方法：move(TM_LEFT | TM_RIGHT | TM_DOWN);
 */
void move(tetMove direction);


/*
 * 函数名称：drop
 * 函数原型：void drop()
 * 功能描述：将正在下落的方块直接降落到底部，并开启下一轮
 * 副作用？：直接改变全局变量falling；加快当前游戏进程
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：drop();
 */
void drop();


/*
 * 函数名称：collisionCheck
 * 函数原型：tetBoundStatu collisionCheck()
 * 功能描述：对正在下落的方块进行碰撞检测
 * 副作用？：无
 *
 * 参数描述：无
 * 返回类型：tetBoundStatu，表示方块碰撞检测结果
 *        （枚举类型tetBoundStatu定义见本文件）
 * 可能取值：
 *      FREEMOVE    - 未碰撞
 *      COLLIDED    - 与堆中方块或界面底部碰撞
 *      LEFT_BOUND  - 与左边界碰撞
 *      RIGHT_BOUND - 与右边界碰撞
 * 注：在多种状态同时成立时，比如，同时与方块堆和左边界碰撞
 *    优先返回左右边界值
 * --------------
 * 使用方法：tetBoundStatu bs = collisionCheck();
 */
tetBoundStatu collisionCheck();


/*
 * 函数名称：moveToTop
 * 函数原型：void moveToTop(tetBlock* b)
 * 功能描述：将指针指向的方块移动到屏幕上方，生成新方块时使用
 * 副作用？：改变传入指针指向的方块坐标
 *
 * 参数描述：方块指针 b :: tetBlock*
 * 返回类型：无
 * --------------
 * 使用方法：moveToTop(&yourBlock);
 */
void moveToTop(tetBlock* b);


/*
 * 函数名称：moveToNextBox
 * 函数原型：void moveToNextBox(tetBlock* b)
 * 功能描述：将指针指向的方块移动到显示下一个方块的盒内
 * 副作用？：改变传入指针指向的方块坐标
 *
 * 参数描述：方块指针 b :: tetBlock*
 * 返回类型：无
 * --------------
 * 使用方法：moveToNextBox(&yourBlock);
 */
void moveToNextBox(tetBlock* b);


/*
 * 函数名称：moveToHoldBox
 * 函数原型：void moveToHoldBox(tetBlock* b)
 * 功能描述：将指针指向的方块移动到显示暂存方块的盒内
 * 副作用？：改变传入指针指向的方块坐标
 *
 * 参数描述：方块指针 b :: tetBlock*
 * 返回类型：无
 * --------------
 * 使用方法：moveToHoldBox(&yourBlock);
 */
void moveToHoldBox(tetBlock* b);

#endif