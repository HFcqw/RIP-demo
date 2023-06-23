/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_ROUTERS 10
#define MAX_NETWORKS 10
#define INFINITY 16
typedef struct {
    unsigned short destNetwork;
    unsigned short nextHop;
    unsigned short metric;
} RoutingTableEntry;
typedef struct {
    unsigned short id;
    unsigned short numRouters;
    unsigned short numNetworks;
    unsigned short network[MAX_NETWORKS];
    RoutingTableEntry routingTable[MAX_ROUTERS][MAX_NETWORKS];
} Router;

void initializeRouter(Router* router, unsigned short id, unsigned short numNetworks, unsigned short networks[]) {
    router->id = id;
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
void updateRoutingTable(Router* router, unsigned short senderId, unsigned short numNetworks, unsigned short networks[], unsigned short metrics[]) {
    for (int i = 0; i < numNetworks; i++) {
        unsigned short network = networks[i];
        unsigned short metric = metrics[i];
        // 查找网络在路由表中的位置
        int index = -1;
        for (int j = 0; j < router->numNetworks; j++) {
            if (router->network[j] == network) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            // 毒性逆转
            if (senderId == router->routingTable[senderId][index].nextHop && metric == INFINITY) {
                router->routingTable[senderId][index].metric = INFINITY;
            }
            else {
                // 更新路由表条目
                unsigned short currentMetric = router->routingTable[senderId][index].metric;
                if (metric + 1 < currentMetric) {
                    router->routingTable[senderId][index].nextHop = senderId;
                    router->routingTable[senderId][index].metric = metric + 1;
                }
            }
        }
    }
}
void displayRoutingTable(Router* router) {
    printf("Router %hu Routing Table:\n", router->id);
    printf("Destination   Next Hop   Metric\n");
    for (int i = 0; i < router->numNetworks; i++) {
        printf("%-14hu %-10hu %-6hu\n", router->routingTable[router->id][i].destNetwork, router->routingTable[router->id][i].nextHop, router->routingTable[router->id][i].metric);
    }
}

int main() {
    unsigned short numNetworks = 3;
    unsigned short networks[] = { 1, 2, 3 };
    Router router;
    initializeRouter(&router, 0, numNetworks, networks);
    unsigned short senderId = 1;
    unsigned short senderNumNetworks = 3;
    unsigned short senderNetworks[] = { 1, 2, 3 };
    unsigned short senderMetrics[] = { 1, 2, 3 };
    updateRoutingTable(&router, senderId, senderNumNetworks, senderNetworks, senderMetrics);
    displayRoutingTable(&router);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>

#define MAX_ROUTERS 10
#define MAX_NETWORKS 20

typedef struct Neighbor {
    int neighborID;
    int distance;
    struct Neighbor* next;
} Neighbor;

typedef struct RoutingTableEntry {
    int networkID;
    int nextHop;
    int distance;
} RoutingTableEntry;

typedef struct Router {
    int routerID;
    Neighbor* neighbors;
    RoutingTableEntry routingTable[MAX_NETWORKS];
} Router;

typedef struct Network {
    Router routers[MAX_ROUTERS];
    int routerCount;
} Network;

void initNetwork(Network* network) {
    network->routerCount = 0;
}

void addRouter(Network* network, int routerID) {
    if (network->routerCount < MAX_ROUTERS) {
        Router* router = &network->routers[network->routerCount];
        router->routerID = routerID;
        router->neighbors = NULL;
        network->routerCount++;
    }
}

void addNeighbor(Router* router, int neighborID, int distance) {
    Neighbor* neighbor = (Neighbor*)malloc(sizeof(Neighbor));
    neighbor->neighborID = neighborID;
    neighbor->distance = distance;
    neighbor->next = router->neighbors;
    router->neighbors = neighbor;
}

void updateRoutingTable(Router* router, int networkID, int nextHop, int distance) {
    int i;
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == networkID) {
            router->routingTable[i].nextHop = nextHop;
            router->routingTable[i].distance = distance;
            return;
        }
    }
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == 0) {
            router->routingTable[i].networkID = networkID;
            router->routingTable[i].nextHop = nextHop;
            router->routingTable[i].distance = distance;
            return;
        }
    }
}

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

void triggerUpdate(Network* network, int routerID) {
    // 发送路由更新信息给邻居路由器
    // ...
}

int selectBestRoute(Router* router, int networkID) {
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
*/