/**************************/
/*  �ļ���:passmaze.h     */
/*  leomaokai  2020.11.21 */
/**************************/
#ifndef PASSMAZE_H
#define PASSMAZE_H

#include <iostream>
#include <Windows.h>
#include <stack>
#include <queue>
#include <vector>
#include <conio.h>	  //������ؿ�
#include <graphics.h> //ͼ�λ�����ؿ�
using namespace std;
#pragma warning(disable : 4996) //�رվ���
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
	void setmaze();	  //�����Թ�
	void initmaze();  //��ʼ���Թ�
	void movemessi(); //�ƶ�С��
	void isrecover();
	void recover();
	void dfspass(); //������ȱ���
	void bfspass(); //������ȱ���
	void newsetout();
	void dfsprint();
	void bfsprint(); //bfs��ӡ
private:
	int key;			  //���ܰ���
	int mode;			  //����ģʽ
	IMAGE img;			  //ͼƬ����
	int entryx;			  //���x����
	int entryy;			  //���y����
	int exitx;			  //����x����
	int exity;			  //����y����
	int entrycount;		  //��ڸ���
	int exitcount;		  //���ڸ���
	int wall[22][22]{};	  //ǽ���� 0��ʾǽ,1��ʾͨ��
	int mazelong;		  //�Թ���
	int mazewide;		  //�Թ���
	int messix;			  //С������x
	int messiy;			  //С������y
	node nodedirect[5]{}; //0 center 1 2 3 4 ��������
	stack<node> mystack;  //������ȱ������õ���ջ
	queue<node> myqueue;  //������ȱ����õ��Ķ���
	vector<node> myvec;	  //������ȱ����õ�������
};

#endif
