/**************************/
/*  文件名:passmaze.h     */
/*  leomaokai  2020.11.21 */
/**************************/
#ifndef PASSMAZE_H
#define PASSMAZE_H

#include <iostream>
#include <Windows.h>
#include <stack>
#include <queue>
#include <vector>
#include <conio.h>	  //键盘相关库
#include <graphics.h> //图形绘制相关库
using namespace std;
#pragma warning(disable : 4996) //关闭警告
struct node
{
	int x;
	int y;
	int flag;
};
class maze
{
public:
	maze();
	void run();
	void setmaze();	  //设置迷宫
	void initmaze();  //初始化迷宫
	void movemessi(); //移动小人
	void isrecover();
	void recover();
	void dfspass(); //深度优先遍历
	void bfspass(); //广度优先遍历
	void newsetout();
	void dfsprint();
	void bfsprint(); //bfs打印
private:
	int key;			  //接受按键
	int mode;			  //运行模式
	IMAGE img;			  //图片对象
	int entryx;			  //入口x坐标
	int entryy;			  //入口y坐标
	int exitx;			  //出口x坐标
	int exity;			  //出口y坐标
	int entrycount;		  //入口个数
	int exitcount;		  //出口个数
	int wall[22][22]{};	  //墙数组 0表示墙,1表示通过
	int mazelong;		  //迷宫长
	int mazewide;		  //迷宫宽
	int messix;			  //小人坐标x
	int messiy;			  //小人坐标y
	node nodedirect[5]{}; //0 center 1 2 3 4 东南西北
	stack<node> mystack;  //深度优先遍历所用到的栈
	queue<node> myqueue;  //广度优先遍历用到的队列
	vector<node> myvec;	  //广度优先遍历用到的数组
};

#endif
