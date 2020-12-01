/**************************/
/*  �ļ���:passmaze.cpp   */
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
	//960*660�����ܴ�С  30*30����һ������
	//660*660�Թ���С ʣ�½���Ϊ��ʾ�� �Թ����Ϊ22*22����
	initgraph(960, 660);
	setbkcolor(LIGHTCYAN); //���ñ�����ɫ��ɫ
	loadimage(&img, L"messi.jpg", 31, 31);
}
void maze::run()
{
	cleardevice();
	initmaze();
	setmaze();
	if (mode == 1)
		//dfspass();//�������
		bfspass();//����������·��
	else
		return;
	while (1)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 27) //�˳�
				exit(0);
			if (key == 114) //���¿�ʼ
				return;
		}
	}
}

//��ʼ���Թ�
void maze::initmaze()
{
	cleardevice();

	//�����Թ��ĳ��Ϳ�
	wchar_t l[10];
	wchar_t w[10];
	InputBox(l, 10, L"�������Թ��ĳ�(1~20)");
	InputBox(w, 10, L"�������Թ��Ŀ�(1~20)");
	mazelong = _wtoi(l);
	mazewide = _wtoi(w);
	if (mazelong > 20 || mazelong < 1 || mazewide > 20 || mazewide < 1)
	{
		outtextxy(480, 330, L"long or wide error and 5s exit");
		Sleep(5000);
		exit(0);
	}

	myvec.resize(22 * 22); //��ʼ��bfs��ʹ�õĸ�������
	//ĳһ��(x,y)��Ӧ�����е� [ x * row + y ],myvec����ÿ�����ǰ����

	//������ǽ ����x����y����x����y
	setfillcolor(LIGHTGREEN);							 //��Χǽ���Ϊ��ɫ
	solidrectangle(0, 0, (22 - mazelong) / 2 * 30, 660); //��ǽ
	if (mazelong % 2 == 0)
		solidrectangle(660 - (22 - mazelong) / 2 * 30, 0, 660, 660); //��ǽ
	else
		solidrectangle(660 - (22 - mazelong) / 2 * 30 - 30, 0, 660, 660); //��ǽ ������ǽ��ˢһ��
	solidrectangle(0, 0, 660, (22 - mazewide) / 2 * 30);				  //��ǽ
	if (mazewide % 2 == 0)
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30, 660, 660); //��ǽ
	else
		solidrectangle(0, 660 - (22 - mazewide) / 2 * 30 - 30, 660, 660); //��ǽ ������ǽ��ˢһ��
	//���Ƹ��� (x1,y1)---(x2,y2)
	setcolor(BLUE);
	for (int i = 0; i < 22; ++i)
	{
		line(i * 30, 0, i * 30, 660);
		line(0, i * 30, 660, i * 30);
	}

	//������ʾ��Ϣ
	RECT r = {710, 40, 900, 300}; // ��������
	setbkmode(TRANSPARENT);		  //���ֱ���Ϊ͸��
	//setcolor(BLUE);//����Ϊ��ɫ
	setfont(15, 0, L"����"); //���ָ�ʽ
	drawtext(L"Maokai maze \n\n\
	a: �����Թ����\n\n\
	s: ���յ�\n\n\
	w: ���ǽ\n\n\
	q: ɾ��ǽ,�յ㣬���\n\n\
	e: ��ɺ���ʾ\n\n\
	i k j l ��������\n\n\
	Esc:  ���˳�",
			 &r, DT_WORDBREAK); //97 115 119 113 101
								//105 107 106 108 27
	setfillcolor(LIGHTGREEN);
	newsetout();
	outtextxy(730, 400, L"�Թ�״̬: ");
	outtextxy(810, 400, L"�ȴ�����");
	outtextxy(750, 450, L"��r(�˳���ʾ)���¿�ʼ"); //114

	//��ʼ��С��λ��
	messix = (22 - mazelong) / 2;
	messiy = (22 - mazewide) / 2;
	putimage(messix * 30, messiy * 30, &img);

	//��ʼ��ǽ����
	for (int i = 0; i < mazewide; ++i)
		for (int j = 0; j < mazelong; ++j)
			wall[i + (22 - mazewide) / 2][j + (22 - mazelong) / 2] = 1;
}
void maze::newsetout() //��ʼ����ʾ��
{
	setfillcolor(LIGHTCYAN);
	solidrectangle(810, 400, 960, 440);
}

//�����Թ�
void maze::setmaze()
{
	mode = 0; //ģʽ0�����Թ�
	while (1)
	{
		if (kbhit())
		{
			newsetout();
			outtextxy(810, 400, L"���ڻ���");
			key = getch();
			//messi�ƶ�������Խ��
			if (key == 105) //i��
			{
				isrecover();
				movemessi();
				messiy--;
				if (messiy <= (22 - mazewide) / 2) //��ֹԽ��
					messiy = (22 - mazewide) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 107) //k��
			{
				isrecover();
				movemessi();
				messiy++;
				if (messiy >= (22 - mazewide) / 2 + mazewide - 1)
					messiy = (22 - mazewide) / 2 + mazewide - 1;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 106) //j��
			{
				isrecover();
				movemessi();
				messix--;
				if (messix <= (22 - mazelong) / 2)
					messix = (22 - mazelong) / 2;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}
			if (key == 108) //l��
			{
				isrecover();
				movemessi();
				messix++;
				if (messix >= (22 - mazelong) / 2 + mazelong - 1)
					messix = (22 - mazelong) / 2 + mazelong - 1;
				putimage(messix * 30, messiy * 30, &img);
				recover();
			}

			//����ǽ
			//ע��:��ͼ��x��Ӧ������y,y��Ӧx
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

			//ɾ��ǽ������յ�
			//ע��:��ͼ��x��Ӧ������y,y��Ӧx
			if (key == 113)
			{
				if (getpixel(messix * 30 + 15, messiy * 30 + 15) == RED) //ɾ��ǽ
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					wall[messiy][messix] = 1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == GREEN) //ɾ���յ�
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					exitcount--; //�յ�����һ
					exitx = -1;
					exity = -1;
				}
				else if (getpixel(messix * 30 + 15, messiy * 30 + 15) == YELLOW) //ɾ�����
				{
					setfillcolor(LIGHTCYAN);
					solidrectangle(messix * 30, messiy * 30, (messix + 1) * 30, (messiy + 1) * 30);
					entrycount--; //�������һ
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

			//������ںͳ���
			if (key == 97 && entrycount == 0) //�����Ϊ0
			{
				setfillcolor(YELLOW);
				solidcircle(messix * 30 + 15, messiy * 30 + 15, 10); //���ƻ�ɫʵ��Բ
				entrycount++;										 //��ڸ�����һ
				entryx = messix;									 //���x����ΪС��x����
				entryy = messiy;
			}
			if (key == 115 && exitcount == 0) //������Ϊ0
			{
				setfillcolor(GREEN);
				solidcircle(messix * 30 + 15, messiy * 30 + 15, 10);
				exitcount++; //���ڸ�����һ
				exitx = messix;
				exity = messiy;
			}

			if (key == 101)
			{
				mode = 1; //1ģʽͨ���Թ�
				break;
			}
			if (key == 27) //�˳�
				exit(0);
			if (key == 114)
				return;
		}
	}
}

//�ƶ�messi����
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
void maze::isrecover() //�ж��ƶ�messi�Ƿ�Ҫ�ָ�����״̬
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
void maze::recover() //���ݱ�ǻָ�����״̬
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

//ͨ���Թ�������DFS��
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
		outtextxy(810, 400, L"������ʾ");
	}

	isrecover(); //��С���ƶ������λ��
	movemessi();
	recover();
	putimage(entryx * 30, entryy * 30, &img);
	bool success = false;	   //�ж��Ƿ񵽴��յ�
	int zx[4] = {1, 0, -1, 0}; //4������ ��������
	int zy[4] = {0, 1, 0, -1};
	node anode;		  //����ڵ�
	int x = 0, y = 0; //��ʱ����
	anode.x = entryx;
	anode.y = entryy;
	mystack.push(anode); //�����ջ

	while (!success && !mystack.empty()) //û���ҵ��յ㲢��ջ��Ϊ��
	{
		node head = mystack.top(); //�õ�ջ������ڵ�
		int i = 0;				   //�ӵ�һ������ʼ�ж�
		while (1)
		{
			x = head.x + zx[i]; //�õ�ջ�������4����������
			y = head.y + zy[i];
			node newnode; //������ʱ�ڵ��¼����
			newnode.x = x;
			newnode.y = y;
			if (wall[y][x] == 1) //ĳ��������·
			{
				if (x != exitx || y != exity) //���յ�
				{
					mystack.push(newnode);	  //���˷�������ڵ�������ջ
					wall[head.y][head.x] = 0; //��ջ���ڵ���Ϊǽ
					break;					  //�ص���һ��while�ж�,ȡ�µ�ջ������
				}
				else if (x == exitx && y == exity) //�յ�
				{
					mystack.push(newnode); //���˷�������ڵ���ջ
					success = true;		   //�ɹ������յ�
					break;				   //�ص���һ��while�жϻ��˳�ѭ��
				}
			}
			else if (wall[y][x] == 0) //������ǽ
			{
				if (i == 3) //4�������жϹ����Ҷ���ǽ
				{
					wall[head.y][head.x] = 0; //��ջ���ڵ���Ϊǽ
					mystack.pop();			  //ջ���ڵ��ջ,�ҽڵ�ÿ�������жϹ��Ҷ��߲�ͨ
					break;					  //�˳�����ѭ��
				}
				++i; //�����ж���������
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

//��ӡ·��
void maze::dfsprint()
{
	//����һ��ջ����DFSջ�е�����ڵ�(��ȷ�Ĵ���㵽�յ��·��)
	stack<node> tempstack;
	while (!mystack.empty())
	{
		node tempnode = mystack.top();
		tempstack.push(tempnode);
		mystack.pop();
	}
	//��ӡ
	while (!tempstack.empty())
	{
		node tempnode = tempstack.top(); //�õ�ջ���ڵ�����
		int x = tempnode.x;
		int y = tempnode.y;
		isrecover(); //�ƶ�С�˵�ջ���ڵ�
		movemessi();
		circle(messix * 30 + 15, messiy * 30 + 15, 5); //���ƿ���ԲȦ
		recover();
		messix = x;
		messiy = y;

		putimage(x * 30, y * 30, &img);
		Sleep(500);		 //��ʱ0.5��
		tempstack.pop(); //ջ����ջ
	}
	putimage(exitx * 30, exity * 30, &img); //���С���ƶ�������λ��
}

//������ȱ���
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
		outtextxy(810, 400, L"������ʾ");
	}

	isrecover(); //��С���ƶ������λ��
	movemessi();
	recover();
	putimage(entryx * 30, entryy * 30, &img);

	bool success = false;	   //�ж��Ƿ񵽴��յ�
	int zx[4] = {1, 0, -1, 0}; //4������ ��������
	int zy[4] = {0, 1, 0, -1};
	node anode;		  //����ڵ�
	int x = 0, y = 0; //��ʱ����
	anode.x = entryx;
	anode.y = entryy;
	myqueue.push(anode);				 //������
	while (!success && !myqueue.empty()) //û���ҵ��յ㲢�Ҷ��в�Ϊ��
	{
		node tempnode = myqueue.front();  //ȡ��ͷ�ڵ�
		wall[tempnode.y][tempnode.x] = 0; //���ڵ�λ����Ϊǽ
		for (int i = 0; i < 4; ++i)
		{
			node tempnode1; //�˽ڵ����������ͷ�ڵ���ĸ�����
			tempnode1.x = tempnode.x + zx[i];
			tempnode1.y = tempnode.y + zy[i];
			if (wall[tempnode1.y][tempnode1.x] == 1) //�����·
			{
				//��ĳ������ڵ��ǰ���ڵ���Ϊ��ͷ�ڵ�
				myvec[tempnode1.y * mazewide + tempnode1.x] = tempnode;
				myqueue.push(tempnode1);						  //���������ڵ����
				if (tempnode1.x == exitx && tempnode1.y == exity) //������յ�
				{
					success = true; //�ɹ��ҵ��յ�,�˳�whileѭ��
					break;
				}
			}
		}
		myqueue.pop(); //��ͷ����,�ж���һ������ڵ���ĸ�����
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

//bfs��ӡ·��
void maze::bfsprint()
{
	stack<node> tempstack;
	int x = exitx;
	int y = exity;
	node tempnode;
	tempnode.x = exitx;
	tempnode.y = exity;
	tempstack.push(tempnode); //�Ƚ��յ���ջ

	while (x != entryx || y != entryy) //��·���ϵĽڵ���յ㵽���������ջ
	{
		node tempnode = myvec[y * mazewide + x];
		tempstack.push(tempnode);
		x = tempnode.x;
		y = tempnode.y;
	}
	while (!tempstack.empty())
	{
		node tempnode = tempstack.top(); //�õ�ջ���ڵ�����
		int x = tempnode.x;
		int y = tempnode.y;
		isrecover(); //�ƶ�С�˵�ջ���ڵ�
		movemessi();
		circle(messix * 30 + 15, messiy * 30 + 15, 5); //���ƿ���ԲȦ
		recover();
		messix = x;
		messiy = y;

		putimage(x * 30, y * 30, &img);
		Sleep(500);		 //��ʱ0.5��
		tempstack.pop(); //ջ����ջ
	}
}