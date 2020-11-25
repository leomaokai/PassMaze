PassMaze 迷宫通行,DFS

使用到的图形库

[EasyX 2020版](https://easyx.cn/downloads/)

 开发环境:

平台:Windows 10 

开发工具:Visual Studio 2019

第一次用编写这样的游戏图形界面,代码写得比较乱

编制一个求解迷宫通路的图形界面演示程序

问题描述：

1）输入一个任意大小的迷宫，任设起点、终点、障碍，用栈求出一条走出迷宫的路径，并显示在屏幕上。

2）根据用户界面提示，用键盘输入。home键设置迷宫起点，end键设终点，上下左右箭头键移动，enter键添加墙，del键删除墙，完成后按F9键演示，Esc键退出。(为了方便操作与键盘兼容,a键设置起点,s键设终点,w键添加墙,q键删除墙,e键开始演示)

3）橙色的实心小圆圈表示起点，绿色实心圆圈表示终点，空心圆圈表示足迹，红色方块表示墙。

4）本程序只求出一条成功的通路，但若对求解函数MazePath稍加更改即可求得全部路径。此外，因受图形界面限制，不能保存或载入测试文件（此功能可在Maze_text中实现）。

5）当为输入起点时，消息显示“Error：You must set Startplace.”;未输入终点时，显示"Error: You must set Endplace." 找到路径时，屏幕显示足迹，并在消息框出现Path found，否则消去足迹，显示Path not found.


