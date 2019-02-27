#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <fstream>
#include <string.h>
#include "stdio.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

#define MAX_PATH 20
#define MAX_SPOT 20
#define MAX2 2
// 邻接表
class ListUDG
{
    private:
        //顶点对应的邻接点
        class ENode
        {
            public:
                int ivex;//指向的顶点的位置
                string dis;
                ENode *nextEdge;//指向下一条弧
        };

        // 邻接表中表的顶点
        class VNode
        {
            public:
                string spotName;//景点名
                string spotInfo;//信息
                string spotStar;//欢迎度（星级）
                string ifExistRest;//是否有休息区
                string ifExistWC;//是否有公厕
                ENode *firstEdge;//指向第一条依附该顶点的弧
        };

	private:
        int mVexNum;//顶点的数目
        int mEdgNum;//边的数目
        VNode mVexs[MAX_SPOT];//暂时存放所有景点的数组

    public:
        int spot_num=0;//统计当前有多少景点
        vector<string>note;//存放公告
		//ListUDG();
        ListUDG(string vexs[][5], int vlen, string edges[][3], int elen,int spots);//将数组的内容放置到邻接链表
		~ListUDG();

        void writeToFile();//更新内容写入文件
        void addIntoArr();//构建邻接矩阵（二维数组形式）
        void print();//打印邻接矩阵
        void printSpot();//打印景点信息
        void printNote();//打印公告
        void addEdg();//增加道路
        void addSpot();//增加景点
        void removeEdg();//删除道路
        void removeSpot();//删除景点
        void findWord();//模糊查找（关键字）
        void sortSpot();//景点欢迎度排序
        void findShortestPath();//最短距离，最短路径
        void guideLine();//生成导游路线
        void DIJ(int v0, int p[][MAX_SPOT],int D[]);//迪杰斯特拉算法
        //void Floyd();//Floyd算法
        //void print_path();//用Floyd算法打出
        void DFS(int c);//深度优先算法
        bool judgeSpotExist(string spot);//判断某经典是否存在

	private:
        int getPosition(string ch);//返回当前景点在数组中位置
        void linkLast(ENode *list, ENode *node);// 将node节点加入到链表
        void destroyLink(VNode *list, ENode *node);//在链表中删除node节点
};

ListUDG* init();//初始化（包含读文件）
void menu1(ListUDG* pG);//打印主菜单
void menuAdmin(ListUDG* pG);//打印管理员菜单
void parkinglot();//打印停车场菜单

//车辆信息
class park{
public:
    string number;
    int hour;
    int minute;
    int position;
};

bool isInZan(string zan[], string number);//判断车是否在栈内
int indexZ(string z[], string n);//返回车在栈哪位
#endif

