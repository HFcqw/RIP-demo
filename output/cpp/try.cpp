#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROUTERS 26	//RIP网络中最大顶点（路由器）数
#define MAX_NETWORKS 24	//设每个路由器最多有24个lan接口，最多连24个局域网
#define INFINITY 16	//一条通路上最多可包含的路由器数量是15个,16跳中止

typedef char RouterType;	//顶点类型（路由器名字）
typedef int EdgeType;	//边上的权值类型（RIP网络固定为1）

typedef struct Neighbor {	//边表结点（邻接路由器）
	int neighborID;	//邻接点域，存储该顶点对应的下标
	EdgeType distance;	//权值，对于非网图可以不需要
	struct Neighbor* next;	//指向下一个邻接点
} Neighbor;

typedef struct RoutingTableEntry {	//路由表 
	int networkID;	//目的网络地址 
	int nextHop;	//下一跳 
	int distance;	//距离
}RTable;

typedef struct Router { //顶点表结点（路由器）
	RouterType routerID;
	int neighborCount;
	Neighbor* neighbors;	//边表头指针firstedge
	int networkCount;
	//int networkID[MAX_NETWORKS];
	RoutingTableEntry routingTable[MAX_NETWORKS];
} Router;

typedef struct Network {
	Router routers[MAX_ROUTERS];
	int routerCount;
} Network;

/*********
* 算法设计
* 初始化：创建网络和路由器，设置初始的路由表。
* 更新路由表：接收邻居路由器发送的路由更新信息，根据接收到的信息更新本地路由表。
* 触发更新：根据一定的策略（例如时间间隔或事件触发）触发路由器发送路由更新信息给邻居路由器。
* 选择最佳路由：根据路由表中的距离信息选择最佳的下一跳路由器。
*********/

/*
* 初始化：创建网络和路由器，设置初始的路由表。（手动输入+读取文件）
*/

void initNetwork(Network* network) {    // 初始化路由表
    network->routerCount = 0;
}

void addRouter(Network* network, int routerID) {    // 添加路由器
    if (network->routerCount < MAX_ROUTERS) {
        Router* router = &network->routers[network->routerCount];
        router->routerID = routerID;
        router->neighbors = NULL;
        network->routerCount++;
    }
}

void addNeighbor(Router* router, int neighborID, int distance) {    // 添加邻居
    Neighbor* neighbor = (Neighbor*)malloc(sizeof(Neighbor));
    neighbor->neighborID = neighborID;
    neighbor->distance = distance;
    neighbor->next = router->neighbors;
    router->neighbors = neighbor;
}

/*
* 更新路由表
*/

void updateRoutingTable(Router* router, int networkID, int nextHop, int distance) { 
    int i;
    // 查找路由表中是否已存在目标网络的条目
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == networkID) {   // 如果找到目标网络条目，更新距离和下一跳
            router->routingTable[i].nextHop = nextHop; 
            router->routingTable[i].distance = distance;
            return;
        }
    }
    // 如果路由表中不存在目标网络的条目，添加新条目
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == 0) {
            router->routingTable[i].networkID = networkID;
            router->routingTable[i].nextHop = nextHop;
            router->routingTable[i].distance = distance;
            return;
        }
    }
}

/*
* 处理路由信息更新
*/

void processRoutingUpdate(Router* router, int senderID, int networkID, int distance) { 
    Neighbor* neighbor = router->neighbors;
    while (neighbor != NULL) {
        if (neighbor->neighborID == senderID) {
            int newDistance = distance + neighbor->distance;
            updateRoutingTable(router, networkID, senderID, newDistance);
            return;
        }
        neighbor = neighbor->next;
    }
}

/*
* 发送路由更新信息给邻居路由器
*/

void triggerUpdate(Network* network, int routerID) { 
    Router* router = NULL;
    int i;

    // 查找指定路由器ID的路由器
    for (i = 0; i < network->routerCount; i++) {
        if (network->routers[i].routerID == routerID) {
            router = &network->routers[i];
            break;
        }
    }

    if (router == NULL) {
        printf("Router with ID %d not found\n", routerID);
        return;
    }

    // 遍历邻居列表，向每个邻居发送路由更新信息
    Neighbor* neighbor = router->neighbors;
    while (neighbor != NULL) {
        int neighborID = neighbor->neighborID;
        int networkID, distance; // 路由更新信息的目标网络ID和距离

        // 根据路由表中的信息构造路由更新信息
        // ...

        // 发送路由更新信息给邻居路由器
        // ...

        neighbor = neighbor->next;
    }
}

int selectBestRoute(Router* router, int networkID) {    //选择最佳路由
    int i;
    int minDistance = -1;
    int bestRoute = -1;
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == networkID) {
            if (minDistance == -1 || router->routingTable[i].distance < minDistance) {
                minDistance = router->routingTable[i].distance;
                bestRoute = router->routingTable[i].nextHop;
            }
        }
    }
    return bestRoute;
}

int main() {
    Network network;
    initNetwork(&network);

    // 添加路由器
    addRouter(&network, 1);
    addRouter(&network, 2);

    // 添加邻居
    addNeighbor(&network.routers[0], 2, 1);
    addNeighbor(&network.routers[1], 1, 1);

    // 处理路由更新
    processRoutingUpdate(&network.routers[0], 2, 10, 1);

    // 选择最佳路由
    int nextHop = selectBestRoute(&network.routers[0], 10);
    printf("Next hop for network 10: %d\n", nextHop);

    return 0;
}