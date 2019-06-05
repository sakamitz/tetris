/*
 * 项目：Tetris
 * 文件名：fileIO.c
 * 概览：文件处理模块
 * -------------
 * 负责功能：排行榜及游戏存档处理，包括
 *      排行榜分数 读取/存入
 *      多游戏存档 读取/写入
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "exception.h" // 需要进行错误处理

#include "tetris.h"  // 需要得知方块结构声明
#include "layout.h"  // 需要取得方块堆大小
#include "fileIO.h"  // 本模块

/* 引入流程控制模块 flow.c 中的变量
 * 这些变量将作为游戏存档的内容
 */
extern int speed;  // 游戏速度
extern int score;  // 当前分数
extern int level;  // 当前难度等级，最高为5
extern int lines;  // 已消除的行数

// 方块暂存状态
extern bool OnHolding;
extern bool OnRelease;
extern bool delayOneRound;

// 已落下的方块堆
extern tetBoard stored[MaxCoordY+1][MaxCoordX+1];
extern tetBlock falling;    // 正在下落的方块
extern tetBlock nextBlock;  // 下一个下落方块
extern tetBlock holdBlock;  // 当前暂存的方块


#define NewPointer(T) (T*)malloc(sizeof(T))

// 定义带错误处理的读取宏
#define SafeRead(ptr, size, nmemb, stream) \
        { \
            if(fread(ptr, size, nmemb, stream) < nmemb) \
                raise(InvalidFormat); \
        }

/*
 * 函数名：freeRecords
 * -------------
 * 释放传入的排行榜分数链表
 * 参数类型：结构 tetRecord，其定义见fileIO.h
 */
void freeRecords(tetRecord* rec)
{
    tetRecord* tmp;

    while (rec != NULL)
    {
        tmp = rec;
        rec = rec->next;
        free(tmp);
    }
}

/*
 * 函数名：loadRecords
 * -------------
 * 读入所有排行榜分数，返回第一个记录结点；无数据则返回NULL
 * 返回类型：结构 tetRecord，其定义见fileIO.h
 */
tetRecord* loadRecords()
{
    // 游戏分数记录路径为 saves 目录下的 records 文件
    static char* recordFile = "saves\\records";
    tetRecord *head = NewPointer(tetRecord), *current, *tmp;
    // 为方便处理，从头结点的下一个节点开始存放数据
    head->next = NULL;

    FILE* fp = fopen(recordFile, "r");

    // 打开文件失败
    if (fp == NULL)
        return NULL;

    current = head;
    // 循环读入所有分数，直到文件结尾
    while (!feof(fp))
    {
        // 读入一个分数记录
        tmp = NewPointer(tetRecord);
        tmp->next = NULL;
        fscanf(fp, "%s", &(tmp->name));
        fscanf(fp, "%d", &(tmp->score));

        // 到达文件结尾时读取到的分数为0，返回
        // （这依赖于fscanf处理 %d 时遇到非数字字符的行为）
        if (tmp->score == 0)
            break;

        // 继续读取下一个节点
        current->next = tmp;
        current = current->next;
    }

    return head->next;
}


/*
 * 函数名：saveRecord
 * -------------
 * 向排行榜存入一个分数
 * 参数类型：char* 玩家名，int 玩家分数
 * 返回类型：int，代表保存是否成功
 *          成功，返回 SUCCESS
 *          失败，返回 FAILURE
 *          （这两个宏定义于fileIO.h）
 */
int saveRecord(char* name, int score)
{
    // 游戏分数记录路径为 saves 目录下的 records 文件
    static char* recordFile = "saves\\records";
    // 读取之前的排行榜数据
    tetRecord* original = loadRecords();

    FILE* fp = fopen(recordFile, "w");

    // 创建文件失败
    if (fp == NULL)
        return FAILURE;

    // 如果之前无数据，则直接存入
    if (original == NULL)
    {
        fprintf(fp, "%s %d\n", name, score);
        fclose(fp);
        return SUCCESS;
    }
    // 之前有数据，则插入到合适的位置
    else
    {
        // 标记待写入记录是否已存入
        int flagSaved = 0;
        tetRecord* current = original;
        while (current != NULL)
        {
            // 如果待写入数据已写入，或当前位置不适合，则写入原有记录
            if (flagSaved || current->score > score)
            {
                fprintf(fp, "%s %d\n", current->name, current->score);
                current = current->next;
            }
            // 到达合适的位置，写入待存数据（分数相同时后来者居上）
            else
            {
                fprintf(fp, "%s %d\n", name, score);
                flagSaved = 1;
            }
        }
        // 如果原有数据已全部写入，但待存数据仍未写入
        // 即待存数据为最后一名的情况
        if (flagSaved == 0)
            fprintf(fp, "%s %d\n", name, score);

        fclose(fp);
        return SUCCESS;
    }
}

/*
 * 函数名：loadGame
 * -------------
 * 读取一个玩家的游戏存档
 * 参数类型：char* 玩家名
 * 返回类型：int，代表读取是否成功
 *          成功，返回 SUCCESS
 *          失败，返回 FAILURE
 *          （这两个宏定义于fileIO.h）
 */
int loadGame(char* username)
{
    // 存档路径为 saves 目录下的 $(用户名).save 文件
    static char filename[18];
    sprintf(filename, "saves\\%s.save", username);

    // 以二进制读取方式打开文件
    FILE* fp = fopen(filename, "rb");

    // 没有该存档，或无法访问
    if (fp == NULL)
        return FAILURE;

    // 定义exception：无效或已损坏存档
    exception InvalidFormat;
    try
    {
        // 读入已落下的方块堆
        SafeRead(&stored, (MaxCoordY+1)*(MaxCoordX+1)*sizeof(tetBoard), 1, fp);

        SafeRead(&falling,   sizeof(tetBlock), 1, fp);  // 读入正在下落的方块信息
        SafeRead(&nextBlock, sizeof(tetBlock), 1, fp);  // 读入即将下落的方块信息
        SafeRead(&holdBlock, sizeof(tetBlock), 1, fp);  // 读入暂存的方块信息

        SafeRead(&speed, sizeof(int), 1, fp);  // 读入游戏速度
        SafeRead(&score, sizeof(int), 1, fp);  // 读入游戏分数
        SafeRead(&level, sizeof(int), 1, fp);  // 读入游戏难度等级
        SafeRead(&lines, sizeof(int), 1, fp);  // 读入已消除行数

        // 读入hold状态
        SafeRead(&OnHolding,     sizeof(bool), 1, fp);
        SafeRead(&OnRelease,     sizeof(bool), 1, fp);
        SafeRead(&delayOneRound, sizeof(bool), 1, fp);

    // 处理存档格式错误
    except(InvalidFormat)
        fclose(fp);
        return FAILURE;
    } endtry

    fclose(fp);
    return SUCCESS;
}

/*
 * 函数名：saveGame
 * -------------
 * 保存玩家的游戏存档
 * 参数类型：char* 玩家名
 * 返回类型：int，代表保存是否成功
 *          成功，返回 SUCCESS
 *          失败，返回 FAILURE
 *          （这两个宏定义于fileIO.h）
 */
int saveGame(char* username)
{
    // 存档路径为 saves 目录下的 $(用户名).save 文件
    static char filename[18];
    sprintf(filename, "saves\\%s.save", username);

    // 创建目录saves，如果存在则什么都不做
    mkdir("saves");

    // 以二进制写入方式打开文件
    FILE* fp = fopen(filename, "wb");

    // 文件打开失败
    if (fp == NULL)
        return FAILURE;

    // 保存已落下的方块堆
    fwrite(&stored, (MaxCoordY+1)*(MaxCoordX+1)*sizeof(tetBoard), 1, fp);

    fwrite(&falling,   sizeof(tetBlock), 1, fp);  // 保存正在下落的方块信息
    fwrite(&nextBlock, sizeof(tetBlock), 1, fp);  // 保存即将下落的方块信息
    fwrite(&holdBlock, sizeof(tetBlock), 1, fp);  // 保存暂存的方块信息

    fwrite(&speed, sizeof(int), 1, fp);  // 保存游戏速度
    fwrite(&score, sizeof(int), 1, fp);  // 保存游戏分数
    fwrite(&level, sizeof(int), 1, fp);  // 保存游戏难度等级
    fwrite(&lines, sizeof(int), 1, fp);  // 保存已消除行数

    // 保存hold状态
    fwrite(&OnHolding,     sizeof(bool), 1, fp);
    fwrite(&OnRelease,     sizeof(bool), 1, fp);
    fwrite(&delayOneRound, sizeof(bool), 1, fp);

    fclose(fp);
    return SUCCESS;
}