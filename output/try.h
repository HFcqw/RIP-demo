#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <malloc.h>
#include <time.h>
#include<assert.h>

// 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define MAX_ROUTERS 20	//设RIP网络中路由器数最大为20
#define MAX_NETWORKS 20	//设网络数最大为10
#define MAX_ENTRIES 25	//一个RIP报文最多可包含25个路由
#define INFINITY 16	//一条通路上最多可包含的路由器数量是15个,16跳中止

typedef int Status;  // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int Boolean; // Boolean是布尔类型,其值是TRUE或FALSE
typedef char RouterType;	//顶点类型（路由器名字）
typedef char NetworkType;
typedef int EdgeType;	//边上的权值类型（RIP网络固定为1）

typedef struct RoutingTable {	//路由表结构体 
	char networkID;	//目的网络地址（局域网编号） 
	char nextHop;	//下一跳 
	int distance;	//距离
} RTable;

typedef struct Neighbor {	//边表结点（邻接路由器）
	int neighborID;	//邻接点域，存储该顶点对应的下标
	EdgeType distance;	//边上的权值，固定为1
	struct Neighbor* next;	//指向下一个邻接点/下一条弧 的指针
} Neighbor, * LinkList;

typedef struct Router { //顶点表结点（节点仅考虑路由器，不考虑直连网络）
	RouterType routerID;
	int neighborCount;
	int dirnetworkCount;	//有几个直连网络
	NetworkType dirNetworkID[MAX_NETWORKS];	//与路由器直连的网络
	Neighbor* neighbors;	//边表头指针firstedge，指向第一条依附于该顶点的弧的指针
	RTable* routingTable;	//路由表数组
} Router, RouterList[MAX_ROUTERS];

typedef struct Network {	//相邻路由器组成的图，不将直连网络作为节点
	RouterList RL;  //存放路由器结点的顺序表
	//DNList DL;	//存放目的网络信息
	int routerCount;    //路由器数量
	int edgeCount;    //路由器直连而成的图的边的数量
	int networkCount;	//网络的总数
} Network;

/*
typedef struct DestNetwork {	//目的网络的结构体
	NetworkType dnetworkID;
	int dirrouterCount;	//有几个直连的网络
	RouterType routerID[MAX_ROUTERS];	//与网络直连的路由器
	char* road;
	RTable routingTable;
}DNet, DNList[MAX_NETWORKS];
*/

/*********
********** 数据结构设计
* 路由表
* 邻接路由器链表（边结点）
* 路由器（顶点结点）,每个单链表的第一个结点存放有关顶点的信息，把这个结点看做链表的表头，其余结点存放有关边的信息
* 目的网络
* 全局网络
********** 算法设计
* * * 图的基本操作
* 图的存储
* 添加节点、修改节点配置信息、删除节点
* 添加边、删除边
* 销毁图
* * * * RIP网络核心算法
* 初始化：创建网络和路由器（创建图），设置初始的路由表
* 更新路由表：接收邻居路由器发送的路由更新信息，根据接收到的信息更新本地路由表。
* 触发更新：根据一定的策略（例如时间间隔或事件触发）触发路由器发送路由更新信息给邻居路由器
* 选择最佳路由：根据路由表中的距离信息选择最佳的下一跳路由器
* 打印路由表
*********/


/*
设计数据结构：

定义表示路由器的结构体，包括路由器ID、邻居列表和路由表等信息。
定义表示邻居路由器的结构体，包括邻居ID和邻居接口等信息。
定义表示路由表项的结构体，包括目标网络ID、下一跳路由器ID、距离和更新时间等信息。
定义表示网络的结构体，包括路由器列表和网络ID等信息。

实现从文件读取网络配置：
设计函数来从文件中读取网络配置信息，并根据读取到的信息构建网络数据结构。配置文件可以包含路由器ID、邻居列表、路由表等信息。

实现路由信息更新和维护算法：
设计函数来处理路由信息的更新和维护。可以根据RIP协议规范，使用距离向量算法更新路由表。
在路由信息更新过程中，更新邻居列表、路由表和路由器的状态等信息。

实现路由选择算法：
设计函数来根据路由表选择最佳路由。可以根据距离和其他因素选择最优路径。
*/

/*
标准文档 RFC 1058
要求自治系统内的每一个路由器都要维护从它自己到自治系统内其他每一个网络的距离记录，这组距离称为距离向量
RIP使用跳数来衡量到达目的网络的距离
路由器到直连网络的距离定义为1（思科路由器定义为0）
路由器到非直连网络的距离定义为所经过的路由器数加1
一条路径最多只能包含15个路由器，“距离”等于16时相当于不可达
选择路由器数量最少的路由
等价负载均衡
路由器仅和相邻路由器交换信息，交换自己的路由表，周期性交换（每30秒发送一次RIP更新报文）
*/