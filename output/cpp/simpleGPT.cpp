/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_ROUTERS 10
#define MAX_NETWORKS 20
#define INFINITY 16

// 路由器数据结构
typedef struct {
    int routerID;
    int neighborCount;
    int neighborIDs[MAX_ROUTERS];
} Router;

// 路由表数据结构
typedef struct {
    int networkID;
    int nextHop;
    int distance;
} RoutingTableEntry;

// 路由表
RoutingTableEntry routingTable[MAX_NETWORKS];
int routingTableSize = 0;

void initializeRouter(Router* router, unsigned short id, unsigned short numNetworks, unsigned short networks[]) {
    router->routerID = id;
    router->numNetworks = numNetworks;
    for (int i = 0; i < numNetworks; i++) {
        router->network[i] = networks[i];
    }
    // 初始化路由表，将自己所连接的网络添加到路由表中
    for (int i = 0; i < numNetworks; i++) {
        router->routingTable[id][i].destNetwork = networks[i];
        router->routingTable[id][i].nextHop = id;
        router->routingTable[id][i].metric = 0;
    }
}
// 更新路由表
void updateRoutingTable(int networkID, int nextHop, int distance) {
    // 查找路由表中是否已存在目标网络的条目
    int i;
    for (i = 0; i < routingTableSize; i++) {
        if (routingTable[i].networkID == networkID) {
            // 如果找到目标网络条目，更新距离和下一跳
            routingTable[i].nextHop = nextHop;
            routingTable[i].distance = distance;
            return;
        }
    }

    // 如果路由表中不存在目标网络的条目，添加新条目
    if (routingTableSize < MAX_NETWORKS) {
        routingTable[routingTableSize].networkID = networkID;
        routingTable[routingTableSize].nextHop = nextHop;
        routingTable[routingTableSize].distance = distance;
        routingTableSize++;
    }
}

// 处理路由信息更新
void processRoutingUpdate(Router* router, int networkID, int senderID, int distance) {
    // 如果发送者是邻居路由器，更新路由表
    int i;
    for (i = 0; i < router->neighborCount; i++) {
        if (router->neighborIDs[i] == senderID) {
            // 计算距离（可以根据RIP协议规则进行计算）
            int newDistance = distance + 1;

            // 更新路由表
            updateRoutingTable(networkID, senderID, newDistance);
            break;
        }
    }
}

// 选择最佳路由
int selectBestRoute(int networkID) {
    int i;
    int minDistance = -1;
    int bestRoute = -1;

    // 遍历路由表，查找目标网络的最佳路由
    for (i = 0; i < routingTableSize; i++) {
        if (routingTable[i].networkID == networkID) {
            if (minDistance == -1 || routingTable[i].distance < minDistance) {
                minDistance = routingTable[i].distance;
                bestRoute = routingTable[i].nextHop;
            }
        }
    }

    return bestRoute;
}

void displayRoutingTable(Router* router) {
    printf("Router %hu Routing Table:\n", router->id);
    printf("Destination   Next Hop   Metric\n");
    for (int i = 0; i < router->numNetworks; i++) {
        printf("%-14hu %-10hu %-6hu\n", router->routingTable[router->id][i].destNetwork, router->routingTable[router->id][i].nextHop, router->routingTable[router->id][i].metric);
    }
}
int main() {
    // 创建路由器实例并初始化
    Router router;
    router.routerID = 1;
    router.neighborCount = 2;
    router.neighborIDs[0] = 2;
    router.neighborIDs[1] = 3;

    // 模拟处理路由信息更新
    processRoutingUpdate(&router, 10, 2, 1);
    processRoutingUpdate(&router, 20, 3, 2);
}
*/