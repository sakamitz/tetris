/*
 * 项目：Tetris
 * 文件名：fileIO.h
 * 概览：文件处理模块
 * -------------
 * 主要内容：
 *      分数记录数据结构定义
 *      排行榜分数 读取/写入函数声明
 *      多游戏存档 读取/写入函数声明
 *
 * 外部接口：
 *      排行榜分数处理函数：
 *          freeRecords
 *          loadRecords
 *          saveRecord
 *      游戏存档处理函数：
 *          loadGame
 *          saveGame
 */

#ifndef FILEIO_H
#define FILEIO_H

#define NAMELEN 11  // 用户名长度限制为10

#define SUCCESS  1  // 文件IO处理结果定义
#define FAILURE  0

typedef struct tetRecord tetRecord;

// 分数记录链表结构
struct tetRecord
{
  char       name[NAMELEN]; // 用户名
  int        score; // 分数
  tetRecord* next;
};


/*
 * 函数名称：freeRecords
 * 函数原型：void freeRecords(tetRecord* rec)
 * 功能描述：释放传入的排行榜分数链表
 * 副作用？：改变传入指针指向的内存块
 *
 * 参数描述：待释放链表头结点指针 rec :: tetRecord*
          （结构tetRecord定义见本文件）
 * 返回类型：无
 * --------------
 * 使用方法：freeRecords(yourRecordsPtr);
 */
void freeRecords(tetRecord* rec);


/*
 * 函数名称：loadRecords
 * 函数原型：tetRecord* loadRecords()
 * 功能描述：读入所有排行榜分数，返回第一个记录结点
 *         | 无记录数据则返回NULL
 *         | 记录文件为 saves/records
 * 副作用？：引起磁盘读取
 *
 * 参数描述：无
 * 返回类型：tetRecord*，分数记录的头结点指针
 * --------------
 * 使用方法：tetRecord* recPtr = loadRecords();
 */
tetRecord* loadRecords();


/*
 * 函数名称：saveRecord
 * 函数原型：int saveRecord(char* name, int score)
 * 功能描述：向排行榜存入一个分数
 *         | 记录文件为 saves/records
 * 副作用？：引起磁盘写入
 *
 * 参数描述：玩家名  name :: char*
 *         玩家分数 score :: int
 * 返回类型：int，代表保存是否成功
 *          成功，返回 SUCCESS
 *          失败，返回 FAILURE
 *          （宏定义见本文件）
 * --------------
 * 使用方法：int savedOrNot = saveRecord(username, score);
 */
int saveRecord(char* name, int score);


/*
 * 函数名称：loadGame
 * 函数原型：int loadGame(char* username)
 * 功能描述：读取一个玩家的游戏存档，自带错误处理
 *         | 存档目录为 saves/
 *         | 文件名为 $(username).save
 * 副作用？：引起磁盘读取、引起游戏状态及数据的改变
 *
 * 参数描述：玩家名 name :: char*
 * 返回类型：int，代表读取结果
 *          成功       返回 SUCCESS
 *          读取失败    返回 FAILURE
 *          （宏定义见本文件）
 * --------------
 * 使用方法：int loadedOrNot = loadGame(username);
 */
int loadGame(char* username);


/*
 * 函数名称：saveGame
 * 函数原型：int saveGame(char* username)
 * 功能描述：保存玩家的游戏存档
 *         | 存档目录为 saves/
 *         | 文件名为 $(username).save
 * 副作用？：引起磁盘写入
 *
 * 参数描述：玩家名 name :: char*
 * 返回类型：int，代表写入是否成功
 *          成功，返回 SUCCESS
 *          失败，返回 FAILURE
 *          （宏定义见本文件）
 * --------------
 * 使用方法：int savedOrNot = saveGame(username);
 */
int saveGame(char* username);

#endif