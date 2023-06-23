/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_ROUTERS 10
#define MAX_NETWORKS 20
#define INFINITY 16

// ·�������ݽṹ
typedef struct {
    int routerID;
    int neighborCount;
    int neighborIDs[MAX_ROUTERS];
} Router;

// ·�ɱ����ݽṹ
typedef struct {
    int networkID;
    int nextHop;
    int distance;
} RoutingTableEntry;

// ·�ɱ�
RoutingTableEntry routingTable[MAX_NETWORKS];
int routingTableSize = 0;

void initializeRouter(Router* router, unsigned short id, unsigned short numNetworks, unsigned short networks[]) {
    router->routerID = id;
    router->numNetworks = numNetworks;
    for (int i = 0; i < numNetworks; i++) {
        router->network[i] = networks[i];
    }
    // ��ʼ��·�ɱ����Լ������ӵ�������ӵ�·�ɱ���
    for (int i = 0; i < numNetworks; i++) {
        router->routingTable[id][i].destNetwork = networks[i];
        router->routingTable[id][i].nextHop = id;
        router->routingTable[id][i].metric = 0;
    }
}
// ����·�ɱ�
void updateRoutingTable(int networkID, int nextHop, int distance) {
    // ����·�ɱ����Ƿ��Ѵ���Ŀ���������Ŀ
    int i;
    for (i = 0; i < routingTableSize; i++) {
        if (routingTable[i].networkID == networkID) {
            // ����ҵ�Ŀ��������Ŀ�����¾������һ��
            routingTable[i].nextHop = nextHop;
            routingTable[i].distance = distance;
            return;
        }
    }

    // ���·�ɱ��в�����Ŀ���������Ŀ���������Ŀ
    if (routingTableSize < MAX_NETWORKS) {
        routingTable[routingTableSize].networkID = networkID;
        routingTable[routingTableSize].nextHop = nextHop;
        routingTable[routingTableSize].distance = distance;
        routingTableSize++;
    }
}

// ����·����Ϣ����
void processRoutingUpdate(Router* router, int networkID, int senderID, int distance) {
    // ������������ھ�·����������·�ɱ�
    int i;
    for (i = 0; i < router->neighborCount; i++) {
        if (router->neighborIDs[i] == senderID) {
            // ������루���Ը���RIPЭ�������м��㣩
            int newDistance = distance + 1;

            // ����·�ɱ�
            updateRoutingTable(networkID, senderID, newDistance);
            break;
        }
    }
}

// ѡ�����·��
int selectBestRoute(int networkID) {
    int i;
    int minDistance = -1;
    int bestRoute = -1;

    // ����·�ɱ�����Ŀ����������·��
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
    // ����·����ʵ������ʼ��
    Router router;
    router.routerID = 1;
    router.neighborCount = 2;
    router.neighborIDs[0] = 2;
    router.neighborIDs[1] = 3;

    // ģ�⴦��·����Ϣ����
    processRoutingUpdate(&router, 10, 2, 1);
    processRoutingUpdate(&router, 20, 3, 2);
}
*/