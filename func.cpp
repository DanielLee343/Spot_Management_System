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
#include "head.h"

using namespace std;

#define MAX_PATH 20
#define MAX_SPOT 20
#define MAX2 2
#define INFINITY 32767

//ListUDG::ListUDG(){
//}

ListUDG::ListUDG(string vexs[][5], int vlen, string edges[][3], int elen,int spots){
    string c1, c2, c3,dis;
    int i, p1, p2;
    ENode *node1, *node2;

    // 初始化"顶点数"和"边数"
    mVexNum = vlen;
    mEdgNum = elen;
    spot_num = spots;
    // 初始化顶点
    for(i=0; i<mVexNum; i++){
        mVexs[i].spotName = vexs[i][0];
        mVexs[i].spotInfo = vexs[i][1];
        mVexs[i].spotStar = vexs[i][2];
        mVexs[i].ifExistRest = vexs[i][3];
        mVexs[i].ifExistWC = vexs[i][4];
        mVexs[i].firstEdge = NULL;
    }

    // 初始化的边
    for(i=0; i<mEdgNum; i++){
        // 读取边的起始顶点和结束顶点
        c1 = edges[i][0];
        c2 = edges[i][1];
        c3 = edges[i][2];

        p1 = getPosition(c1);
        p2 = getPosition(c2);

        node1 = new ENode();
        node1->ivex = p2;
        node1->dis = c3;
        // 将node1链接到"p1所在链表的末尾"
        if(mVexs[p1].firstEdge == NULL)
            mVexs[p1].firstEdge = node1;
        else
            linkLast(mVexs[p1].firstEdge, node1);

        node2 = new ENode();
        node2->ivex = p1;
        node2->dis = c3;
        if(mVexs[p2].firstEdge == NULL)
            mVexs[p2].firstEdge = node2;
        else
            linkLast(mVexs[p2].firstEdge, node2);
    }
}

ListUDG::~ListUDG(){
}

void ListUDG::linkLast(ENode *list, ENode *node){
    ENode *p = list;

    while(p->nextEdge){
        p = p->nextEdge;
    }
    p->nextEdge = node;
    node->nextEdge = nullptr;
}

//储存邻接链表的邻接矩阵（用二维数组）


void ListUDG::destroyLink(VNode *list, ENode *node){
    ENode *p = list->firstEdge;//p是当前链表的第一个连接景点
    if(p->ivex == node->ivex){//如果第一个节点就是要删除的
        list->firstEdge = list->firstEdge->nextEdge;//删除第一个节点
        return;
    }
    p = p->nextEdge;//将p移到第二个连接景点节点处
    ENode *previous = list->firstEdge;//previous就是第一个连接景点
    while(p->ivex != node->ivex){//如果node不是p，直到找到node=p
        if(p->nextEdge==nullptr){//并且是最后一个
            break;//跳出while
        }
        p = p->nextEdge;//如果不是最后一个，一直往后移
        previous = previous->nextEdge;//同理
    }
    if(p->nextEdge!=nullptr){//如果不是最后一个
        previous->nextEdge = p->nextEdge;//删除节点
    }
    else{
        previous->nextEdge = NULL;
    }
    p = NULL;
}

int ListUDG::getPosition(string ch){
    int i;
    for(i=0; i<mVexNum; i++)
        if(mVexs[i].spotName==ch)
            return i;
    return -1;
}

int arr[MAX_SPOT][MAX_SPOT];

void ListUDG::writeToFile(){
    //更新景点信息文件
    string spotName[spot_num];
    string spotInfo[spot_num];
    string spotStar[spot_num];
    string ifExistRest[spot_num];
    string ifExistWC[spot_num];
    ofstream out;
    out.open("景点信息更改.txt");
    for(int i = 0;i<spot_num;i++){
        spotName[i] = mVexs[i].spotName;
        spotInfo[i] = mVexs[i].spotInfo;
        spotStar[i] = mVexs[i].spotStar;
        ifExistRest[i] = mVexs[i].ifExistRest;
        ifExistWC[i] = mVexs[i].ifExistWC;
        out<<spotName[i]<<" "<<spotInfo[i]<<" "<<spotStar[i]<<" "<<ifExistRest[i]<<" "<<ifExistWC[i]<<endl;
    }
    out.close();

    //更改道路信息文件
    addIntoArr();
    string spot1[MAX_PATH];
    string spot2[MAX_PATH];
    int dis[MAX_PATH];
    int count = 0;
    out.open("道路信息更改.txt");
    for(int i = 0;i<spot_num;i++){
        for(int j = i+1;j<spot_num;j++){
            if(arr[i][j] != INFINITY){
                spot1[count] = mVexs[i].spotName;
                spot2[count] = mVexs[j].spotName;
                dis[count] = arr[i][j];
                count++;
            }
        }
    }
    for(int i = 0;i<count;i++){
        out<<spot1[i]<<" "<<spot2[i]<<" "<<dis[i]<<endl;
    }
    out.close();
}

void ListUDG::addIntoArr(){
    ENode *node;
    //初始化所有元素为无穷大
    for(int i = 0;i<spot_num;i++){
        for(int j = 0;j<spot_num;j++){
            arr[i][j] = INFINITY;
        }
    }
    for(int i = 0;i<spot_num;i++){
        node = mVexs[i].firstEdge;
        for(int j = 0;j<spot_num;j++){
            if(i == j){
                arr[i][j] = 0;
            }
            while(node){//赋值
                arr[i][node->ivex] = atoi((node->dis).c_str());
                node = node->nextEdge;
            }
        }
    }
}

void ListUDG::print(){
    addIntoArr();
//    ENode *node;
//    cout << "邻接矩阵如下:" << endl;
//    //这部分是另一种展现形式
//    for (int i = 0; i < spot_num; i++){
//        cout << i << "(" << mVexs[i].spotName << "): ";
//        node = mVexs[i].firstEdge;
//        while (node != NULL)
//        {
//            cout << node->dis << "(" << mVexs[node->ivex].spotName << ") ";
//            node = node->nextEdge;
//        }
//        cout << endl;
//    }
    cout<<"\t";
    for(int i = 0;i<spot_num;i++){
        cout<<mVexs[i].spotName<<"\t";
    }
    cout<<endl;
    for(int i = 0;i<spot_num;i++){
        cout<<mVexs[i].spotName<<"\t";
        for(int j = 0;j<spot_num;j++){
            cout<<arr[i][j]<<"\t";
        }cout<<endl;
    }
    system("pause");
}


void ListUDG::printSpot(){
    cout<<"景点名\t"<<"景点信息\t"<<"星级\t"<<"有无休息区\t"<<"有无公厕"<<endl;
    for (int i = 0; i < spot_num; i++){
        cout<<mVexs[i].spotName<<"\t"<<mVexs[i].spotInfo<<"\t"<<mVexs[i].spotStar<<"\t"<<mVexs[i].ifExistRest<<"\t\t"<<mVexs[i].ifExistWC<<endl;
    }
    system("pause");
}

bool ListUDG::judgeSpotExist(string spot){
    bool flag = false;
    for(int i = 0;i<spot_num;i++){
        if(mVexs[i].spotName == spot){
            flag = true;
            return true;
        }
    }
    if(!flag){
        return false;
    }
}

void ListUDG::addEdg(){
    r3:
    cout<<"请输入要增加道路的两个景点名称:"<<endl;
    string spot1,spot2;
    cin>>spot1>>spot2;
    if(judgeSpotExist(spot1)&&judgeSpotExist(spot2)){
        int p1, p2;
        p1 = getPosition(spot1);
        p2 = getPosition(spot2);
        cout<<"请输入他们之间的距离:"<<endl;
        string dis;
        cin>>dis;

        ENode *node1, *node2;

        node1 = new ENode();
        node1->ivex = p2;
        node1->dis = dis;
        if(mVexs[p1].firstEdge == NULL)
            mVexs[p1].firstEdge = node1;
        else
            linkLast(mVexs[p1].firstEdge, node1);

        node2 = new ENode();
        node2->ivex = p1;
        node2->dis = dis;
        if(mVexs[p2].firstEdge == NULL)
            mVexs[p2].firstEdge = node2;
        else
            linkLast(mVexs[p2].firstEdge, node2);

        writeToFile();

        cout<<"道路插入成功！"<<endl;
        addIntoArr();
        //更新公告
        string note1 = spot1+"和"+spot2+"之间的道路已经建设好，单位距离是"+dis;
        note.push_back(note1);
        system("pause");
    }else{
        cout<<"该景点不存在"<<endl;
        system("pause");
        goto r3;
    }
}

void ListUDG::addSpot(){
    r1:
    cout<<"当前景点数："<<spot_num<<endl;
    cout<<"请输入要增加的景点名称:"<<endl;
    string newSpot;
    cin>>newSpot;
    //不存在新景点名称，再加入
    if(!judgeSpotExist(newSpot)){
        cout<<"请输入该的景点信息:"<<endl;
        string newSpotInfo;
        cin>>newSpotInfo;
        cout<<"请输入景点的星级:"<<endl;
        string newSpotStar;
        cin>>newSpotStar;
        cout<<"请输入景点是否有休息区:(1:有;0:没有)"<<endl;
        int newSpotRest;
        string newSpotResttoString;
        cin>>newSpotRest;
        if(newSpotRest == 1){
            newSpotResttoString = "有";
        }else
            newSpotResttoString = "无";
        cout<<"请输入景点是否有公厕:(1:有;0:没有)"<<endl;
        int newSpotWC;
        string newSpotWCtoString;
        cin>>newSpotWC;
        if(newSpotWC == 1){
            newSpotWCtoString = "有";
        }else{
            newSpotWCtoString = "无";
        }

        mVexs[spot_num].spotName = newSpot;
        mVexs[spot_num].spotInfo = newSpotInfo;
        mVexs[spot_num].spotStar = newSpotStar;
        mVexs[spot_num].ifExistRest = newSpotResttoString;
        mVexs[spot_num].ifExistWC = newSpotWCtoString;

        spot_num++;
        cout<<"景点插入成功！"<<endl;
        //addIntoArr();

        writeToFile();
        string note1 = newSpot+"已建设成功，"+newSpotInfo+"，是"+newSpotStar+"级景点，"+newSpotResttoString+"休息区，"+newSpotWCtoString+"公厕";
        note.push_back(note1);
        system("pause");
    }else{
        cout<<"景点已存在！"<<endl;
        system("pause");
        goto r1;
    }
}

void ListUDG::removeEdg(){
    r4:
    cout<<"请输入要删除的两个景点名称:"<<endl;
    string spot1,spot2;
    cin>>spot1>>spot2;
    if(judgeSpotExist(spot1)&&judgeSpotExist(spot2)){
        int p1, p2;
        p1 = getPosition(spot1);
        p2 = getPosition(spot2);

        ENode *node1, *node2;

        node1 = new ENode();node2 = new ENode();
        node1->ivex = p2;
        node2->ivex = p1;

        VNode * node3 = &mVexs[p1];
        VNode * node4 = &mVexs[p2];
        destroyLink(node3, node1);
        destroyLink(node4, node2);
        cout<<"删除道路成功！"<<endl;

        addIntoArr();
        writeToFile();

        string note1 = spot1+"和"+spot2+"之间的道路正在维护，请绕行...";
        note.push_back(note1);
        system("pause");
    }else{
        cout<<"景点不存在！"<<endl;
        system("pause");
        goto r4;
    }
}

void ListUDG::removeSpot(){
    r2:
    cout<<"请输入要删除的景点名称:"<<endl;
    string spot;
    cin>>spot;
    if(judgeSpotExist(spot)){

        for(int i = 0;i<spot_num;i++){
            if(mVexs[i].spotName==spot){
                    for( int k = 0;k<spot_num;k++){
                    ENode *node = mVexs[k].firstEdge;
                    ENode *previous = mVexs[k].firstEdge;
                    if(node->ivex == i){
                        mVexs[k].firstEdge = node->nextEdge;
                        node = NULL;

                    }else{
                        node = node->nextEdge;
                        while(true){
                            if(node->ivex == i){
                                if(node->nextEdge!=nullptr){
                                    previous->nextEdge = node->nextEdge;
                                }
                                else{
                                    previous->nextEdge=nullptr;
                                }
                                node = NULL;
                                break;
                            }
                            else{
//                        if(node){
//                            break;
//                        }
                        if(node->nextEdge==nullptr){
                            break;
                        }
                        node = node->nextEdge;

                        previous = previous->nextEdge;
                            }

                        }
                    }
                }
                    if(i==spot_num- 1){
                        VNode temp ;
                       mVexs[i]=temp;
                    }
                    else{
                         for(int j = i;j<spot_num;j++){
//                    mVexs[j] = mVexs[j+1];
//                    mVexs[spot_num].firstEdge = NULL;

                    mVexs[j] = mVexs[j+1];

                }
                    }

                spot_num--;
                break;
            }
        }
        cout<<"景点删除成功！"<<endl;
        //addIntoArr();
        writeToFile();
        string note1 = spot+"正在维护，暂不对外开放...";
        note.push_back(note1);
        system("pause");
    }else{
        cout<<"景点不存在！"<<endl;
        system("pause");
        goto r2;
    }
}

//读取文件内容，存入数组
ListUDG* init(){
    int spots = 0;
    ifstream in;
    in.open("道路信息.txt");
    string name1[MAX_PATH],name2[MAX_PATH];
    int dis[MAX_PATH];
    string str[MAX_PATH];
    for(int i = 0;i<MAX_PATH;i++){
        //读取一行
        getline(in,str[i],'\n');
    }
    for(int i = 0;i<MAX_PATH;i++){
        //将每行元素以空格分隔，分别存入数组
        istringstream is(str[i]);
        is>>name1[i]>>name2[i]>>dis[i];
    }
    in.close();

    in.open("景点信息.txt");
    string name[MAX_SPOT];
    string info[MAX_SPOT];
    string star[MAX_SPOT];
    string ifExistRest[MAX_SPOT];
    string ifExistWC[MAX_SPOT];
    string str1[MAX_SPOT];
    for(int i = 0;i<MAX_SPOT;i++){
        getline(in,str1[i],'\n');

    }
    for(int i = 0;i<MAX_SPOT;i++){
        istringstream is(str1[i]);
        is>>name[i]>>info[i]>>star[i]>>ifExistRest[i]>>ifExistWC[i];
        if(name[i]!=""){
            spots++;
        }
    }
    in.close();

    //将刚才的数组放置入二维数组以分类存储
    string vexs[MAX_SPOT][5];
    string edges[MAX_PATH][3];
    for(int i = 0; i< MAX_SPOT;i++){
        vexs[i][0] = name[i];
        vexs[i][1] = info[i];
        vexs[i][2] = star[i];
        vexs[i][3] = ifExistRest[i];
        vexs[i][4] = ifExistWC[i];
    }
    for(int i = 0;i<MAX_PATH;i++){
        edges[i][0] = name1[i];
        edges[i][1] = name2[i];
        edges[i][2] = to_string(dis[i]);
    }
    //景点和边的个数
    int vlen = sizeof(vexs)/sizeof(vexs[0]);
    int elen = sizeof(edges)/sizeof(edges[0]);
    ListUDG* pG;
    //传递给构造函数形成邻接表
    pG = new ListUDG(vexs, vlen, edges, elen,spots);
    return pG;
}

void ListUDG::printNote(){
    for(int i = 0;i<note.size();i++){
        cout<<i+1<<"."<<note[i]<<endl;
    }
    system("pause");
}

//用模糊查找查询关键字
void ListUDG::findWord(){
    cout<<"请输入要查询的关键字"<<endl;
    string word;
    cin>>word;
    bool flag = false;
    for(int i = 0;i<spot_num;i++){
        if(mVexs[i].spotName.find(word)!=string::npos||mVexs[i].spotInfo.find(word)!=string::npos){
            cout<<"景点是"+mVexs[i].spotName+"，"+mVexs[i].spotInfo<<endl;
            flag = true;
            break;
        }
    }
    if(!flag){
        cout<<"没有该景点！"<<endl;
    }
    system("pause");
}

//用冒泡排序排序景点欢迎度
void ListUDG::sortSpot(){
    cout<<"以下是依据景点欢迎度的排序："<<endl;
    string star[spot_num];
    bool flag = false;
    VNode mVexs1[spot_num];
    //不能更改原数组，于是用另外数组承接
    for(int i = 0;i<spot_num;i++){
        mVexs1[i] = mVexs[i];
    }
    //排序
    for(int i = 0;i<spot_num-1;i++){
        for(int j = 0;j<spot_num-1-i;j++){
            if(atoi(mVexs1[j].spotStar.substr(0,1).c_str())>atoi(mVexs1[j+1].spotStar.substr(0,1).c_str())){
                VNode temp = mVexs1[j];
                mVexs1[j] = mVexs1[j+1];
                mVexs1[j+1] = temp;
                flag = true;
            }
        }
    }
    for(int i = spot_num-1;i>=0;i--){
        cout<<mVexs1[i].spotName<<",星级是 "<<mVexs1[i].spotStar<<endl;
    }
    system("pause");
}

void ListUDG::findShortestPath(){
    addIntoArr();
    fsp:
    int cur1,cur2;
    string spot1,spot2;
    int shortestPath[MAX_SPOT][MAX_SPOT];
    int shortestDis[MAX_SPOT];
    cout<<"请输入要搜索最短路径的景点名称："<<endl;
    cin>>spot1>>spot2;
    if(judgeSpotExist(spot1) && judgeSpotExist(spot2)){
        cur1=getPosition(spot1);
        cur2=getPosition(spot2);

        DIJ(cur1, shortestPath, shortestDis);
        cout<<"到各顶点的最短路径及长度为："<<endl;
        if(cur2!=0 && shortestDis[cur2]!=INFINITY){
            cout<<"最短路径为：";
            for(int j=0; j<spot_num; j++){
                if(shortestPath[cur2][j]>-1)
                cout<<mVexs[shortestPath[cur2][j]].spotName<<" ";
            }
            cout<<endl;
            cout<<"最短距离为:"<<shortestDis[cur2]<<endl;
        }else if(shortestDis[cur2]==INFINITY)
            cout<<mVexs[cur1].spotName<<"-"<<mVexs[cur2].spotName<<":"<<"不可达"<<endl;
        system("pause");
    }else{
        cout<<"景点不存在"<<endl;
        system("pause");
        goto fsp;
    }
}

void ListUDG::DIJ(int v0, int shortestPath[][MAX_SPOT],int shortestDis[]){
    int v,w,i,j,min;
    bool final[10];
    for(v = 0;v<spot_num;v++){
        final[v] = false;
        shortestDis[v] = arr[v0][v];
        for(w = 0;w<spot_num;w++){
            shortestPath[v][w] = -1;
        }
        if(shortestDis[v]<INFINITY){
            shortestPath[v][0] = v0;
            shortestPath[v][1] = v;
        }
    }
    shortestDis[v0] = 0;
    final[v0] = true;

    for(i = 0;i<spot_num;i++){
        min = INFINITY;
        for(w = 0;w<spot_num;w++){
            if(!final[w] && shortestDis[w]<min){
                v=w;
                min = shortestDis[w];
            }
        }
        final[v] = true;
        for(w=0; w<spot_num; w++){
            if(!final[w] && min<INFINITY && arr[v][w]<INFINITY && (min+arr[v][w]<shortestDis[w])){
                shortestDis[w]=min+arr[v][w];
                for(j=0; j<spot_num; j++) {
                    shortestPath[w][j]=shortestPath[v][j];
                    if(shortestPath[w][j]==-1){
                        shortestPath[w][j]=w;
                        break;
                    }
                }
            }
        }
    }
}

int passed[MAX_SPOT] = {0};
int np=0;//找到的景点个数
int p[MAX_SPOT] = {0};//表示各个景点的入度值

void ListUDG::DFS(int c){
    np++;//找到的景点数+1
    p[c]++;//这个点的度+1
    if(np == spot_num){//找完了
        return;
    }else{
        cout<<mVexs[c].spotName<<"-->";
    }
    passed[c] = 1;
    for(int i = 0;i<spot_num;i++){
        if(arr[c][i] > 0 && arr[c][i] != INFINITY && passed[i] == 0){
            DFS(i);
            if(spot_num>np){
                cout<<mVexs[c].spotName<<"-->";
                p[c]++;
            }
        }
    }
    if(np == spot_num){
        return;
    }
}

void ListUDG::guideLine(){
    addIntoArr();
    guide:
    cout<<"请输入景点名称："<<endl;
    string spot;
    int j = 0;
    cin>>spot;
    if(judgeSpotExist(spot)){
        for(int i = 0;i<spot_num;i++){
                passed[i]=0;
                p[i]=0;//入度置初值为0
            if(mVexs[i].spotName == spot){
                j = i;
            }
        }
        np=0;
        cout<<"形成的导游线路图（采用深度优先策略）如下所示：\n\n";
        DFS(j);
        system("pause");
    }else{
        cout<<"景点不存在"<<endl;
        system("pause");
        goto guide;
    }
}


stack<park> parking;
stack<park> cars;
queue<park> waits;
string z[MAX2];
//某个车是否在栈内
bool isInZan(string zan[], string number){
    for(int i = 0;i<MAX2;i++){
        if(zan[i] == number){
            return true;
        }
    }
    return false;
}
//n在z[]里面是第几个
int indexZ(string z[], string n){
    for(int i = 0;i<MAX2;i++){
        if(z[i] == n){
            return i;
        }
    }
    return -1;
}

//停入
void goIn(){
    int k1 = 0;
    park car;
    cout<<"请输入车牌号："<<endl;
    cin>>car.number;

    struct tm *newtime;
    time_t long_time;
    time( &long_time);
    newtime = localtime( &long_time);
    int h = newtime->tm_hour;
    int m = newtime->tm_min;
    car.hour = h;
    car.minute = m;

    //获取当前时间
    cout<<"进入停车场时间位：";
    if(car.minute>=1 && car.minute<10){
        cout<<car.hour<<":0"<<car.minute<<endl;
    }else{
        cout<<car.hour<<":"<<car.minute<<endl;
    }

    //是放到停车场还是便道
    if(parking.size()<MAX2){
        for(int i = 0;i<MAX2;i++){
            if(z[i] ==""){
                z[i] = car.number;
                car.position = i;
                break;
            }
        }
        parking.push(car);
        cout<<"该车已进入停车场，在"<<car.position+1<<"号位"<<endl;
    }else{
        cout<<"停车场已满，请等待其他车辆离开:"<<endl;
        waits.push(car);
    }
    cout<<"现在停车场有如下车辆:";
    for(int i = 0;i<MAX2;i++){
        cout<<z[i]<<"  ";
    }
    system("pause");
}
//出停车场
void goOut(){
    if(parking.size() <= 0){
        cout<<"停车场为空!"<<endl;
    }else{
        cout<<"请输入您的车牌号："<<endl;
        string number;
        cin>>number;
        //判断是否在停车场内
        if(isInZan(z, number)){
            //栈顶不是要离开的车
            while(parking.top().number!=number){
                //其他车放到临时栈内
                cars.push(parking.top());
                parking.pop();
            }
            int num = indexZ(z, parking.top().number);
            //取出这个车
            z[num]="";
            struct tm *newtime;
            time_t long_time;
            time( &long_time );
            newtime = localtime( &long_time );
            int h = newtime->tm_hour;
            int m = newtime->tm_min;
            cout<<"车牌号为："<<parking.top().number<<"的车要离开了"<<endl<<"停车时间为： "<<(h*60+m)-(parking.top().hour*60+parking.top().minute)<<"分钟"<<endl<<"停车费用为："<<((h*60+m)-(parking.top().hour*60+parking.top().minute))*5<<"元"<<endl;
            parking.pop();
            //将临时栈的车压到停车场栈
            while(!cars.empty()){
                parking.push(cars.top());
                cars.pop();
            }
            while(parking.size()<MAX2){
                //将等待车压到停车场栈
                if(waits.size()!=0){
                    for(int i = 0;i<MAX2;i++){
                        if(z[i] ==""){
                            z[num] = waits.front().number;
                        }
                    }
                    parking.push(waits.front());
                    waits.pop();
                }
                else{
                    break;
                }
            }
            cout<<"现剩余车辆是："<<endl;
            for(int i = 0;i<MAX2;i++){
                cout<<z[i]<<" ";
            }
        }else{
        cout<<"没有该车辆！请输入正确的车牌号！"<<endl;
        }

    }
    system("pause");
}

void parkinglot(ListUDG* pG){
        cout<<"========================================"<<endl;
        cout<<"            **停车场管理程序**        "<<endl;
        cout<<"                 请选择："<<endl;
        cout<<"========================================"<<endl;
        cout<<"1.进停车场"<<endl;
        cout<<"2.出停车场"<<endl;
        cout<<"0.退出程序"<<endl;
        int choose;
        cin>>choose;
        if(choose ==1 || choose == 2 || choose == 0){
            switch(choose){
            case 1:
                goIn();
                system("cls");
                parkinglot(pG);
                break;
            case 2:
                goOut();
                system("cls");
                parkinglot(pG);
                break;
            case 0:
                system("cls");
                menu1(pG);
                break;
            }
        }else{
            cout<<"输入有误，请重新输入："<<endl;
            system("cls");
            parkinglot(pG);
        }
}

void menu1(ListUDG* pG){
    cout<<"========================================"<<endl;
    cout<<"      欢迎使用景区信息管理系统"<<endl;
    cout<<"         ***请选择菜单***"<<endl;
    cout<<"========================================"<<endl;
    cout<<"1.管理员登录。"<<endl;
    cout<<"2.输出景区景点分布图。"<<endl;
    cout<<"3.输出景区景点信息。"<<endl;
    cout<<"4.景点查找。"<<endl;
    cout<<"5.景点排序。"<<endl;
    cout<<"6.两个景点间的最短路径和最短距离。"<<endl;
    cout<<"7.输出导游路线图。"<<endl;
    cout<<"8.停车场车辆进出记录信息。"<<endl;
    cout<<"9.看公告。"<<endl;
    cout<<"0.退出系统。"<<endl;
    int func;
    cin>>func;
    switch(func){
    case 1:
        system("cls");
        menuAdmin(pG);
        break;
    case 2:
        pG->print();
        system("cls");
        menu1(pG);
        break;
    case 3:
        pG->printSpot();
        system("cls");
        menu1(pG);
        break;
    case 4:
        //景点查找
        pG->findWord();
        system("cls");
        menu1(pG);
        break;
    case 5:
        //景点排序
        pG->sortSpot();
        system("cls");
        menu1(pG);
        break;
    case 6:
        //输出导游路线图
        pG->findShortestPath();
        system("cls");
        menu1(pG);
        break;
    case 7:
        //最短路径
        pG->guideLine();
        system("cls");
        menu1(pG);
        break;
    case 8:
        //停车场
        system("cls");
        parkinglot(pG);
        break;
    case 9:
        pG->printNote();
        system("cls");
        menu1(pG);
        break;
    case 0:
        exit(0);
        break;
    }
}

void menuAdmin(ListUDG* pG){
    cout<<"========================================"<<endl;
    cout<<"          欢迎进入管理员页面"<<endl;
    cout<<"          ***请选择菜单***"<<endl;
    cout<<"========================================"<<endl;
    cout<<"1.插入景点。"<<endl;
    cout<<"2.删除景点。"<<endl;
    cout<<"3.插入路。"<<endl;
    cout<<"4.删除路。"<<endl;
    cout<<"0.返回上一级。"<<endl;
    int admin;
    cin>>admin;
    switch(admin){
    case 1:
        pG->addSpot();
        system("cls");
        menuAdmin(pG);
        break;
    case 2:
        pG->removeSpot();
        system("cls");
        menuAdmin(pG);
        break;
    case 3:
        pG->addEdg();
        system("cls");
        menuAdmin(pG);
        break;
    case 4:
        pG->removeEdg();
        system("cls");
        menuAdmin(pG);
        break;
    case 0:
        system("cls");
        menu1(pG);
        break;
    }
}
