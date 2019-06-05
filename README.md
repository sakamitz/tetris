<div align="center">
<h1>Tetris</h1>
<p>
      <a href="https://github.com/sakamitz/tetris/releases"><img src="https://img.shields.io/badge/build-passing-brightgreen.svg" alt="Releases"></a>
      <a href="https://github.com/sakamitz/tetris/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-MIT-lightgrey.svg" alt="LICENSE"></a>
</p>
</div>

# 简介

基于winAPI，libgraphics 和 SimpleGUI

功能：彩色界面，方块旋转，方块预览，方块暂存，排行榜，交互式界面

# 编译

方法一：现成的工程文件（见 project/）

方法二：GCC + Makefile（如果dev-cpp没有安装在默认路径，需要修改 Makefile 中的 LIBS 和 INCS 变量）

# 使用手册及开发报告

见 report/ 目录

（注：你看到的为公开版本，已隐去其中个人信息）

# 成员

| HSL            | RFJ          | YXN                     |
| -------------- | ------------ | ----------------------- |
| 方块显示       | 方块消除检测 | 模块架构、数据结构设计  |
| 排行榜界面设计 | 帮助界面设计 | Hold 功能、交互系统设计 |
| 代码整理       | 分数记录读取 | 程序报告与用户手册撰写  |
| debug          |              | 代码规范、重构与优化    |

# 开发

接口设计说明(Legacy)：[TetrisInterface](https://www.processon.com/view/link/5cd57ef1e4b06bcc139f7057)

模块分工说明(Legacy)：[TetrisFiles](https://www.processon.com/view/link/5cd649cae4b09b16ffdb592d)

数据结构说明：[TetrisData](https://www.processon.com/view/link/5cd6499ee4b085d010995454)

方块结构说明：[Tetriminos](https://www.processon.com/view/link/5cdf7a55e4b005286487410e)
