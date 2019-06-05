/*
 * 项目：Tetris
 * 文件名：interact.h
 * 概览：用户交互模块
 * -------------
 * 主要内容：
 *      一个通用的交互式对话框框架
 *      各种交互式对话框的声明
 *
 * 外部接口：
 *      通用交互式对话框框架：
 *          MsgBox
 *      一些交互式对话框：
 *          MsgBoxResume
 *          MsgBoxSave
 *          MsgBoxLoad
 *          MsgBoxSuccess
 *          MsgBoxLoadFail
 *          MsgBoxSaveFail
 *          MsgBoxRestart
 *          MsgBoxBackToMain
 *          MsgBoxGameOver
 *          MsgBoxRecord
 */

#ifndef INTERACT_H
#define INTERACT_H

#define HasTextBox 1  // 有文本框
#define NoTextBox  0  // 无文本框


/*
 * 函数名称：MsgBox
 * 函数原型：int MsgBox(char* prompt, char* buttons[], int n,
                        int hasTextBox, char* strBuffer, int buflen);
 * 功能描述：显示一个宽度自适应的交互式对话框，含有文本框输入（可选）
 * 副作用？：界面绘制
 *
 * 参数描述：
 *   prompt :: char*     - 提示消息
 *   buttons[] :: char*  - 按钮列表
 *   n :: int            - 按钮个数
 *   hasTextBox :: int   - 是否含有文本框（0-否 1-是）
 *   strBuffer :: char*  - 文本框的输入缓冲区（如果不用文本框请设为NULL）
 *   buflen :: int       - 文本框缓冲区大小（不用文本框请设为0）
 *
 * 返回类型：int
 *   -1    - 用户没有点击（按下并释放）按钮
 *   >=0   - 用户点击的按钮 index （在buttons[]中）
 *
 * 使用方法：choice = MsgBox(prompt, buttons, n, hasTextBox, strBuffer, buflen);
 * 应用举例：

（无文本框）：
    char* prompt    = "Are you sure to quit?";
    char* buttons[] = {"Ok", "Cancel"};

    int choice = MsgBox(prompt, buttons, 2, NoTextBox, NULL, 0);

    if (choice == 0)      // 点击"Ok"
    {
        quit();
    }
    else if (choice == 1) // 点击"Cancel"
    {
        doSomethingElse();
    }

（有文本框）：
    char* prompt    = "Enter something:";
    char* buttons[] = {"Ok", "Cancel"};
    int MaxLen = 12;
    static char strBuffer[MaxLen];

    int choice = MsgBox(prompt, buttons, 2, HasTextBox, strBuffer, MaxLen);

    // 点击"Ok"且文本框内有内容
    if (choice == 0 && strBuffer[0] != '\0')
    {
        printf("You entered: %s", strBuffer);
    }
    // 点击"Cancel"
    else if (choice == 1)
    {
        doSomethingElse();
    }
 */
int MsgBox(char* prompt, char* buttons[], int n,
            int hasTextBox, char* strBuffer, int buflen);


/*
 * 函数名称：MsgBoxResume
 * 函数原型：void MsgBoxResume()
 * 功能描述：居中显示一个对话框，提示“游戏暂停”
             -------------------
            |                   |
            |    Game paused    |
            |                   |
            |   -------------   |
            |  |   Resume    |  |
            |   -------------   |
             -------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxResume();
 */
void MsgBoxResume();


/*
 * 函数名称：MsgBoxSave
 * 函数原型：void MsgBoxSave()
 * 功能描述：居中显示一个对话框，提示保存分数记录
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
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxSave();
 */
void MsgBoxSave();


/*
 * 函数名称：MsgBoxLoad
 * 函数原型：void MsgBoxLoad()
 * 功能描述：居中显示一个对话框，提示输入游戏存档名，以读取
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
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxLoad();
 */
void MsgBoxLoad();


/*
 * 函数名称：MsgBoxSuccess
 * 函数原型：void MsgBoxSuccess()
 * 功能描述：居中显示一个对话框，提示“操作成功”
             -------------------------------
            |                               |
            |    Operation successfully.    |
            |                               |
            |          ----------           |
            |         |    Ok    |          |
            |          ----------           |
             -------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxSuccess();
 */
void MsgBoxSuccess();


/*
 * 函数名称：MsgBoxLoadFail
 * 函数原型：void MsgBoxLoadFail()
 * 功能描述：居中显示一个对话框，提示读取存档失败
             --------------------------------------------------
            |                                                  |
            |    Record "XXX" doesn't exist or got damaged.    |
            |                                                  |
            |                  ----------                      |
            |                 |    Ok    |                     |
            |                  ----------                      |
             --------------------------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxLoadFail();
 */
void MsgBoxLoadFail();


/*
 * 函数名称：MsgBoxSaveFail
 * 函数原型：void MsgBoxSaveFail()
 * 功能描述：居中显示一个对话框，提示保存（分数记录或存档）失败
          （应该几乎用不到，除非遇到磁盘写入权限等问题）
             ---------------------------------
            |                                 |
            |    Failed to save game data.    |
            |                                 |
            |           ----------            |
            |          |    Ok    |           |
            |           ----------            |
             ---------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxSaveFail();
 */
void MsgBoxSaveFail();


/*
 * 函数名称：MsgBoxRestart
 * 函数原型：void MsgBoxRestart()
 * 功能描述：居中显示一个对话框，提示是否重新开始
             -----------------------------------------
            |                                         |
            |      Discard to start a new game?       |
            |                                         |
            |         --------     --------           |
            |        |   Ok   |   | Cancel |          |
            |         --------     --------           |
             -----------------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxRestart();
 */
void MsgBoxRestart();


/*
 * 函数名称：MsgBoxBackToMain
 * 函数原型：void MsgBoxBackToMain()
 * 功能描述：居中显示一个对话框，提示是否回到主菜单
             -------------------------------------
            |                                     |
            |      Back to the main window?       |
            |                                     |
            |       --------     --------         |
            |      |   Ok   |   | Cancel |        |
            |       --------     --------         |
             -------------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxBackToMain();
 */
void MsgBoxBackToMain();


/*
 * 函数名称：MsgBoxGameOver
 * 函数原型：void MsgBoxGameOver()
 * 功能描述：居中显示一个对话框，提示总得分，及是否保存记录
          （游戏失败时使用）
             --------------------------------------------
            |                                            |
            |      You got XXX points! Save record?      |
            |                                            |
            |           --------     --------            |
            |          |   Ok   |   | Cancel |           |
            |           --------     --------            |
             --------------------------------------------
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxGameOver();
 */
void MsgBoxGameOver();


/*
 * 函数名称：MsgBoxRecord
 * 函数原型：void MsgBoxRecord()
 * 功能描述：居中显示一个对话框，提示输入保存分数时的用户名
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
 * 副作用？：界面绘制
 *
 * 参数描述：无
 * 返回类型：无
 * --------------
 * 使用方法：MsgBoxRecord();
 */
void MsgBoxRecord();

#endif