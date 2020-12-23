/**************************/
/*  文件名:passmaze.cpp   */
/*  leomaokai  2020.11.21 */
/**************************/

#include "passmaze.h"

maze::maze() : key(0),
			   mode(0),
			   entryx(-1),
			   entryy(-1),
			   exitx(-1),
			   exity(-1),
			   mazelong(0),
			   mazewide(0),
			   messix(0),
			   messiy(0),
			   entrycount(0),
			   exitcount(0)
{
	//960*660界面总大小  30*30像素一个格子
	//660*660迷宫大小 剩下界面为提示区 迷宫最多为22*22格子
	initgraph(960, 660);
	setbkcolor(LIGHTCYAN); //设置背景颜色蓝色
	loadimage(&img, L"messi.jpg", 31, 31);
}
void maze::run()
{
	cleardevice();
	initmaze();
	setmaze();
	if (mode == 1)
		//dfspass();//深度优先
		bfspass();//广度优先最短路径
	else
		return;
	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 27) //退出
				exit(0);
			if (key == 114) //重新开始
				return;
		}
	}
}

//初始化迷宫
void maze::initmaze()
{
	cleardevice();

	//设置迷宫的长和宽
	wchar_t l[10];
	wchar_t w[10];
	InputBox(l, 10, L"请输入迷宫的长(1~20)");
	InputBox(w, 10, L"请输入迷宫的宽(1~20)");
	mazelong = _wtoi(l);
	mazewide = _wtoi(w);
	if (mazelong > 20 || mazelong < 1 || mazewide > 20 || mazewide < 1)
	{
		outtextxy(480, 330, L"long or wide error and 5s exit");
		Sleep(5000);
		exit(0);
	}

	myvec.resize(22 * 22); //初始化bfs中使用的辅助数组
	//某一点(x,y)对应数组中的 [ x * row + y ],myvec保存每个点的前驱点

	//绘制外墙 左线x上线y右线x下线y
	setfillcolor(LIGHTGREEN);							 //外围墙填充为绿色
	solidrectangle(0, 0, (22 - mazelong) / 2 * 30, 660); //左墙
	if (mazelong % 2 == 0)
		solidrectangle(660 - (22 - mazelong) / 2 * 30, 0, 660, 660); //右墙
	else
		solidrectangle(660 - (22 - mazelong) / 2 * 30 - 30, 0, 660, 660); //右墙 奇数右墙多刷一格
	solidrectangle(0, 0, 660, (22 - mazewide) / 2 * 30);				  //上墙
	if (mazewide % 2 == 0)
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30, 660, 660); //下墙
	else
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30 - 30, 660, 660); //下墙 奇数下墙多刷一格
	//绘制格子 (x1,y1)---(x2,y2)
	setcolor(BLUE);
	for (int i = 0; i < 22; ++i)
	{
		line(i * 30, 0, i * 30, 660);
		line(0, i * 30, 660, i * 30);
	}

	//绘制提示信息
	RECT r = {710, 40, 900, 300}; // 文字区域
	setbkmode(TRANSPARENT);		  //文字背景为透明
	//setcolor(BLUE);//文字为蓝色
	setfont(15, 0, L"宋体"); //文字格式
	drawtext(L"Maokai maze \n\n\
	a: 设置迷宫起点\n\n\
	s: 设终点\n\n\
	w: 添加墙\n\n\
	q: 删除墙,终点，起点\n\n\
	e: 完成后按演示\n\n\
	i k j l 上下左右\n\n\
	Esc:  键退出",
			 &r, DT_WORDBREAK); //97 115 119 113 101
								//105 107 106 108 27
	setfillcolor(LIGHTGREEN);
	newsetout();
	outtextxy(730, 400, L"迷宫状态: ");
	outtextxy(810, 400, L"等待绘制");
	outtextxy(750, 450, L"按r(退出演示)重新开始"); //114

	//初始化小人位置
	messix = (22 - mazelong) / 2;
	messiy = (22 - mazewide) / 2;
	putimage(messix * 30, messiy * 30, &img);

	//初始化墙数组
	for (int i = 0; i < mazewide; ++i)
		for (int j = 0; j < mazelong; ++j)
			wall[i + (22 - mazewide) / 2][j + (22 - mazelong) / 2] = 1;
}
void maze::newsetout() //初始化提示区
{
	setfillcolor(LIGHTCYAN);
	solidrectangle(810, 400, 960, 440);
}

//设置迷宫
void maze::setmaze()
{
	mode = 0; //模式0设置迷宫
	while (1)
	{
		if (kbhit())
		{
			newsetout();
			outtextxy(810, 400, L"正在绘制");
			key = getch();
			//messi移动但不能越界
			if (key == 105) //i上
			{
				isrecover();
				movemessi();
				messiy--;
				if (messiy <= (22 - mazewide) / 2) //防止越界
					messiy = (22 - mazewide) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 107) //k下
			{
				isrecover();
				movemessi();
				messiy++;
				if (messiy >= (22 - mazewide) / 2 + mazewide - 1)
					messiy = (22 - mazewide) / 2 + mazewide - 1;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 106) //j左
			{
				isrecover();
				movemessi();
				messix--;
				if (messix <= (22 - mazelong) / 2)
					messix = (22 - mazelong) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 108) //l右
			{
				isrecover();
				movemessi();
				messix++;
				if (messix >= (22 - mazelong) / 2 + mazelong - 1)
					messix = (22 - mazelong) / 2 + mazelong - 1;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}

			//设置墙
			//注意:绘图行x对应数组列y,y对应x
			if (key == 119)
			{
				setfillcolor(RED);
				solidrectangle(messix * 30 + 5, messiy * 30 + 5, messix * 30 + 25, messiy * 30 + 25);
				wall[messiy][messix] = 0;
				for (int i = 0; i < 22; ++i)
				{
					line(i * 30, 0, i * 30, 660);
					line(0, i * 30, 660, i * 30);
				}
			}

			//删除墙或起点终点
			//注意:绘图行x对应数组列y,y对应x
			if (key == 113)
			{
				if (getpixel(messix * 30 + 15, messiy * 30 + 15) == RED) //删除墙
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					wall[messiy][messix] = 1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == GREEN) //删除终点
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					exitcount--; //终点数减一
					exitx = -1;
					exity = -1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == YELLOW) //删除起点
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					entrycount--; //起点数减一
					entryx = -1;
					entryy = -1;
				}
				for (int i = 0; i < 22; ++i)
				{
					line(i * 30, 0, i * 30, 660);
					line(0, i * 30, 660, i * 30);
				}
				putimage(messix * 30, messiy * 30, &img);
			}

			//设置入口和出口
			if (key == 97 && entrycount == 0) //入口数为0
			{
				setfillcolor(YELLOW);
				solidcircle(messix * 30 + 15, messiy * 30 + 15, 10); //绘制黄色实心圆
				entrycount++;										 //入口个数加一
				entryx = messix;									 //入口x坐标为小人x坐标
				entryy = messiy;
			}
			if (key == 115 && exitcount == 0) //出口数为0
			{
				setfillcolor(GREEN);
				solidcircle(messix * 30 + 15, messiy * 30 + 15, 10);
				exitcount++; //出口个数加一
				exitx = messix;
				exity = messiy;
			}

			if (key == 101)
			{
				mode = 1; //1模式通行迷宫
				break;
			}
			if (key == 27) //退出
				exit(0);
			if (key == 114)
				return;
		}
	}
}

//移动messi函数
void maze::movemessi()
{
	setfillcolor(LIGHTCYAN);
	solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
	for (int i = 0; i < 22; ++i)
	{
		line(i * 30, 0, i * 30, 660);
		line(0, i * 30, 660, i * 30);
	}
}
const COLORREF color[3] = {YELLOW, GREEN, RED};
void maze::isrecover() //判断移动messi是否要恢复格子状态
{
	nodedirect[0].x = messix * 30;
	nodedirect[0].y = messiy * 30;
	nodedirect[1].x = (messix + 1) * 30;
	nodedirect[1].y = messiy * 30;
	nodedirect[2].x = messix * 30;
	nodedirect[2].y = (messiy + 1) * 30;
	nodedirect[3].x = (messix - 1) * 30;
	nodedirect[3].y = messiy * 30;
	nodedirect[4].x = messix * 30;
	nodedirect[4].y = (messiy - 1) * 30;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
			if (getpixel(nodedirect[i].x + 15, nodedirect[i].y + 15) == color[j])
			{
				nodedirect[i].flag = j + 1;
			}
	}
}
void maze::recover() //根据标记恢复格子状态
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
			if (nodedirect[i].flag == j + 1)
			{
				setfillcolor(color[j]);
				if (j + 1 == 3)
					solidrectangle(nodedirect[i].x + 5, nodedirect[i].y + 5, nodedirect[i].x + 25, nodedirect[i].y + 25);
				else
					solidcircle(nodedirect[i].x + 15, nodedirect[i].y + 15, 10);
			}
	}
	for (int i = 0; i < 22; ++i)
	{
		line(i * 30, 0, i * 30, 660);
		line(0, i * 30, 660, i * 30);
	}
	for (int i = 0; i < 5; ++i)
	{
		nodedirect[i].x = 0;
		nodedirect[i].y = 0;
		nodedirect[i].flag = 0;
	}
}

//通行迷宫函数（DFS）
void maze::dfspass()
{
	newsetout();
	if (entrycount == 0)
	{
		outtextxy(810, 400, L"Error: no entry");
		return;
	}
	else if (exitcount == 0)
	{
		outtextxy(810, 400, L"Error: no exit");
		return;
	}
	else
	{
		outtextxy(810, 400, L"正在演示");
	}

	isrecover(); //将小人移动到起点位置
	movemessi();
	recover();
	putimage(entryx * 30, entryy * 30, &img);
	bool success = false;	   //判断是否到达终点
	int zx[4] = {1, 0, -1, 0}; //4个方向 东南西北
	int zy[4] = {0, 1, 0, -1};
	node anode;		  //坐标节点
	int x = 0, y = 0; //临时坐标
	anode.x = entryx;
	anode.y = entryy;
	mystack.push(anode); //起点入栈

	while (!success && !mystack.empty()) //没有找到终点并且栈不为空
	{
		node head = mystack.top(); //得到栈顶坐标节点
		int i = 0;				   //从第一个方向开始判断
		while (1)
		{
			x = head.x + zx[i]; //得到栈顶坐标的4个方向坐标
			y = head.y + zy[i];
			node newnode; //定义临时节点记录坐标
			newnode.x = x;
			newnode.y = y;
			if (wall[y][x] == 1) //某个方向是路
			{
				if (x != exitx || y != exity) //非终点
				{
					mystack.push(newnode);	  //将此方向坐标节点坐标入栈
					wall[head.y][head.x] = 0; //将栈顶节点设为墙
					break;					  //回到第一次while判断,取新的栈顶坐标
				}
				else if (x == exitx && y == exity) //终点
				{
					mystack.push(newnode); //将此方向坐标节点入栈
					success = true;		   //成功到达终点
					break;				   //回到第一次while判断会退出循环
				}
			}
			else if (wall[y][x] == 0) //方向是墙
			{
				if (i == 3) //4个方向都判断过了且都是墙
				{
					wall[head.y][head.x] = 0; //将栈顶节点设为墙
					mystack.pop();			  //栈顶节点出栈,且节点每个方向都判断过且都走不通
					break;					  //退出本次循环
				}
				++i; //继续判断其它方向
			}
		}
	}
	if (success)
	{
		newsetout();
		outtextxy(810, 400, L"Path found");
		dfsprint();
	}
	else
	{
		newsetout();
		outtextxy(810, 400, L"Path not found");
		return;
	}
	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 114)
			{
				return;
			}
			if (key == 27)
				exit(0);
		}
	}
}

//打印路径
void maze::dfsprint()
{
	//用另一个栈保存DFS栈中的坐标节点(正确的从起点到终点的路径)
	stack<node> tempstack;
	while (!mystack.empty())
	{
		node tempnode = mystack.top();
		tempstack.push(tempnode);
		mystack.pop();
	}
	//打印
	while (!tempstack.empty())
	{
		node tempnode = tempstack.top(); //得到栈顶节点坐标
		int x = tempnode.x;
		int y = tempnode.y;
		isrecover(); //移动小人到栈顶节点
		movemessi();
		circle(messix * 30 + 15, messiy * 30 + 15, 5); //绘制空心圆圈
		recover();
		messix = x;
		messiy = y;

		putimage(x * 30, y * 30, &img);
		Sleep(500);		 //延时0.5秒
		tempstack.pop(); //栈顶出栈
	}
	putimage(exitx * 30, exity * 30, &img); //最后将小人移动到出口位置
}

//广度优先遍历
void maze::bfspass()
{
	newsetout();
	if (entrycount == 0)
	{
		outtextxy(810, 400, L"Error: no entry");
		return;
	}
	else if (exitcount == 0)
	{
		outtextxy(810, 400, L"Error: no exit");
		return;
	}
	else
	{
		outtextxy(810, 400, L"正在演示");
	}

	isrecover(); //将小人移动到起点位置
	movemessi();
	recover();
	putimage(entryx * 30, entryy * 30, &img);

	bool success = false;	   //判断是否到达终点
	int zx[4] = {1, 0, -1, 0}; //4个方向 东南西北
	int zy[4] = {0, 1, 0, -1};
	node anode;		  //坐标节点
	int x = 0, y = 0; //临时坐标
	anode.x = entryx;
	anode.y = entryy;
	myqueue.push(anode);				 //起点入队
	while (!success && !myqueue.empty()) //没有找到终点并且队列不为空
	{
		node tempnode = myqueue.front();  //取队头节点
		wall[tempnode.y][tempnode.x] = 0; //将节点位置设为墙
		for (int i = 0; i < 4; ++i)
		{
			node tempnode1; //此节点用来保存队头节点的四个方向
			tempnode1.x = tempnode.x + zx[i];
			tempnode1.y = tempnode.y + zy[i];
			if (wall[tempnode1.y][tempnode1.x] == 1) //如果是路
			{
				//将某个方向节点的前驱节点设为队头节点
				myvec[tempnode1.y * mazewide + tempnode1.x] = tempnode;
				myqueue.push(tempnode1);						  //将这个方向节点入队
				if (tempnode1.x == exitx && tempnode1.y == exity) //如果是终点
				{
					success = true; //成功找到终点,退出while循环
					break;
				}
			}
		}
		myqueue.pop(); //队头出队,判断下一个方向节点的四个方向
	}
	if (success)
	{
		newsetout();
		outtextxy(810, 400, L"Path found");
		bfsprint();
	}
	else
	{
		newsetout();
		outtextxy(810, 400, L"Path not found");
		return;
	}

	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 114)
			{
				return;
			}
			if (key == 27)
				exit(0);
		}
	}
}

//bfs打印路径
void maze::bfsprint()
{
	stack<node> tempstack;
	int x = exitx;
	int y = exity;
	node tempnode;
	tempnode.x = exitx;
	tempnode.y = exity;
	tempstack.push(tempnode); //先将终点入栈

	while (x != entryx || y != entryy) //将路径上的节点从终点到起点依次入栈
	{
		node tempnode = myvec[y * mazewide + x];
		tempstack.push(tempnode);
		x = tempnode.x;
		y = tempnode.y;
	}
	while (!tempstack.empty())
	{
		node tempnode = tempstack.top(); //得到栈顶节点坐标
		int x = tempnode.x;
		int y = tempnode.y;
		isrecover(); //移动小人到栈顶节点
		movemessi();
		circle(messix * 30 + 15, messiy * 30 + 15, 5); //绘制空心圆圈
		recover();
		messix = x;
		messiy = y;

		putimage(x * 30, y * 30, &img);
		Sleep(500);		 //延时0.5秒
		tempstack.pop(); //栈顶出栈
	}
}