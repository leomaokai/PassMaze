/**************************/
/*  文件名:passmaze.cpp   */
/*  leomaokai  2020.11.21 */
/**************************/

#include"passmaze.h"

maze::maze() :key(0), 
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
	setbkcolor(LIGHTCYAN);//设置背景颜色蓝色
	loadimage(&img, L"messi.jpg", 31, 31);
}
void maze::run()
{
	cleardevice();
	initmaze();
	setmaze();
	if (mode == 1)
		pass();
	else
		return;
	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 27)//退出
				exit(0);
			if (key == 114)//重新开始
				return;
		}
	}
}
void maze::initmaze()//初始化迷宫
{
	cleardevice();

	//设置迷宫的长和宽
	wchar_t	l[10];
	wchar_t w[10];
	InputBox(l, 10, L"请输入迷宫的长(1~20)");
	InputBox(w, 10, L"请输入迷宫的宽(1~20)");
	mazelong = _wtoi(l);
	mazewide = _wtoi(w);
	if (mazelong > 20 || mazelong < 1 || mazewide >20 || mazewide < 1)
	{
		outtextxy(480, 330, L"long or wide error and 5s exit");
		Sleep(5000);
		exit(0);
	}
	//绘制外墙 左线x上线y右线x下线y
	setfillcolor(LIGHTGREEN);//外围墙填充为绿色
	solidrectangle(0, 0, (22 - mazelong) / 2 * 30, 660);//左墙
	if (mazelong % 2 == 0)
		solidrectangle(660 - (22 - mazelong) / 2 * 30, 0, 660,660);//右墙
	else
		solidrectangle(660 - (22 - mazelong) / 2 * 30 - 30, 0, 660, 660);//右墙 奇数右墙多刷一格
	solidrectangle(0, 0, 660, (22 - mazewide) / 2 * 30);//上墙
	if (mazewide % 2 == 0)
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30, 660, 660);//下墙
	else
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30 - 30, 660, 660);//下墙 奇数下墙多刷一格
	//绘制格子 (x1,y1)---(x2,y2)
	setcolor(BLUE);
	for (int i = 0; i < 22; ++i)
	{
		line(i * 30, 0, i * 30, 660);
		line(0, i * 30, 660, i * 30);
	}

	//绘制提示信息
	RECT r = { 710,40,900,300 };// 文字区域
	setbkmode(TRANSPARENT);//文字背景为透明
	//setcolor(BLUE);//文字为蓝色
	setfont(15, 0, L"宋体");//文字格式
	drawtext(L"Maokai maze \n\n\
	a: 设置迷宫起点\n\n\
	s: 设终点\n\n\
	w: 添加墙\n\n\
	q: 删除墙,终点，起点\n\n\
	e: 完成后按演示\n\n\
	i k j l 上下左右\n\n\
	Esc:  键退出",
	 &r,DT_WORDBREAK);//97 115 119 113 101 
					  //105 107 106 108 27
	setfillcolor(LIGHTGREEN);
	newsetout();
	outtextxy(730, 400, L"迷宫状态: ");
	outtextxy(810, 400, L"等待绘制");
	outtextxy(750, 450, L"按r(退出演示)重新开始");//114
	
	//初始化小人位置
	messix = (22 - mazelong) / 2;
	messiy = (22 - mazewide) / 2;
	putimage(messix * 30, messiy * 30, &img);

	//初始化墙数组
	for (int i = 0; i < mazewide; ++i)
		for (int j = 0; j < mazelong; ++j)
			wall[i + (22 - mazewide) / 2 ][j + (22 - mazelong) / 2 ] = 1;
}
void maze::newsetout()
{
	setfillcolor(LIGHTCYAN);
	solidrectangle(810, 400, 960, 440);
}
void maze::setmaze() //设置迷宫
{
	mode = 0;//模式0设置迷宫
	while (1)
	{
		if (kbhit())
		{
			newsetout();
			outtextxy(810, 400, L"正在绘制");
			key = getch();
			//messi移动但不能越界
			if (key == 105)//i上 
			{
				isrecover();
				movemessi();
				messiy--;
				if (messiy <= (22 - mazewide) / 2)
					messiy = (22 - mazewide) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 107)//k下
			{
				isrecover();
				movemessi();
				messiy++;
				if (messiy >= (22 - mazewide) / 2 + mazewide - 1)
					messiy = (22 - mazewide) / 2 + mazewide - 1;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 106)//j左
			{
				isrecover();
				movemessi();
				messix--;
				if (messix <= (22 - mazelong) / 2)
					messix = (22 - mazelong) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 108)//l右
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
				solidrectangle(messix * 30+5, messiy * 30+5, messix * 30 + 25, messiy * 30 + 25);
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
				if (getpixel(messix * 30 + 15, messiy * 30 + 15) == RED)
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					wall[messiy][messix] = 1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == GREEN)
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					exitcount--;
					exitx = -1;
					exity = -1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == YELLOW)
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					entrycount--;
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
			if (key == 97 && entrycount==0)
			{
				setfillcolor(YELLOW);
				solidcircle(messix * 30+15, messiy * 30+15, 10);
				entrycount++;
				entryx = messix;
				entryy = messiy;
			}
			if (key == 115 && exitcount==0)
			{
				setfillcolor(GREEN);
				solidcircle(messix * 30+15, messiy * 30+15, 10);
				exitcount++;
				exitx = messix;
				exity = messiy;
			}
			if (key == 101)
			{
				mode = 1;//1模式通行迷宫
				break;
			}
			if (key == 27)//退出
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
const COLORREF color[3] = { YELLOW,GREEN,RED };
void maze::isrecover()//判断移动messi是否要恢复格子状态
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
		for(int j=0;j<3;++j)
			if (getpixel(nodedirect[i].x + 15, nodedirect[i].y + 15) == color[j])
			{
				nodedirect[i].flag = j+1;
			}
	}
}
void maze::recover()//根据标记恢复格子状态
{
	for (int i = 0; i < 5; ++i)
	{
		for(int j=0;j<3;++j)
		if (nodedirect[i].flag==j+1 )
		{
			setfillcolor(color[j]);
			if (j+1 == 3)
				solidrectangle(nodedirect[i].x+5, nodedirect[i].y+5, nodedirect[i].x + 25, nodedirect[i].y + 25);
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
void maze::pass()
{
	newsetout();
	if (entrycount==0)
	{
		outtextxy(810, 400, L"Error: no entry");
		return;
	}
	else if (exitcount==0)
	{
		outtextxy(810, 400, L"Error: no exit");
		return;
	}
	else
	{
		outtextxy(810, 400, L"正在演示");
	}
	isrecover();
	movemessi();
	recover();
	putimage(entryx * 30, entryy * 30, &img);
	bool success = false;//判断是否到达终点
	int zx[4] = { 1,0,-1,0 };//4个方向 东南西北
	int zy[4] = { 0,1,0,-1 };
	node anode;
	int x = 0, y = 0;
	anode.x = entryx;
	anode.y = entryy;
	//cout << entryx << entryy << endl;
	mystack.push(anode);
	while (!success && !mystack.empty())
	{
		node head = mystack.top();
		//cout << head.x << head.y << endl;
		int i = 0;
		while (i < 4)
		{
			x = head.x + zx[i];
			y = head.y + zy[i];
			node newnode;
			newnode.x = x;
			newnode.y = y;
			if (wall[y][x] == 1)//路
			{
				if (x != exitx || y != exity) //非终点
				{
					mystack.push(newnode);
					wall[head.y][head.x] = 0;
					break;
				}
				else if (x == exitx && y == exity)//终点
				{
					mystack.push(newnode);
					success = true;
					break;
				}
			}
			else if (wall[y][x] == 0 ) //墙
			{
				if (i == 3)
				{
					wall[head.y][head.x] = 0;
					mystack.pop();
					break;
				}
				++i;
			}
		}
	}
	if (success)
	{
		newsetout();
		outtextxy(810, 400, L"Path found");
		print();
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
void maze::print()
{
	stack<node> tempstack;
	while (!mystack.empty())
	{
		node tempnode = mystack.top();
		tempstack.push(tempnode);
		mystack.pop();
	}
	while (!tempstack.empty())
	{
		node tempnode = tempstack.top();
		int x = tempnode.x;
		int y = tempnode.y;
		isrecover();
		movemessi();
		circle(messix * 30 + 15, messiy * 30 + 15, 5);
		recover();
		messix = x;
		messiy = y;
		putimage(x * 30, y * 30, &img);
		Sleep(500);
		tempstack.pop();
	}
	putimage(exitx * 30, exity * 30, &img);
}