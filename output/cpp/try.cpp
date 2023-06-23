#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROUTERS 26	//RIP��������󶥵㣨·��������
#define MAX_NETWORKS 24	//��ÿ��·���������24��lan�ӿڣ������24��������
#define INFINITY 16	//һ��ͨ·�����ɰ�����·����������15��,16����ֹ

typedef char RouterType;	//�������ͣ�·�������֣�
typedef int EdgeType;	//���ϵ�Ȩֵ���ͣ�RIP����̶�Ϊ1��

typedef struct Neighbor {	//�߱��㣨�ڽ�·������
	int neighborID;	//�ڽӵ��򣬴洢�ö����Ӧ���±�
	EdgeType distance;	//Ȩֵ�����ڷ���ͼ���Բ���Ҫ
	struct Neighbor* next;	//ָ����һ���ڽӵ�
} Neighbor;

typedef struct RoutingTableEntry {	//·�ɱ� 
	int networkID;	//Ŀ�������ַ 
	int nextHop;	//��һ�� 
	int distance;	//����
}RTable;

typedef struct Router { //������㣨·������
	RouterType routerID;
	int neighborCount;
	Neighbor* neighbors;	//�߱�ͷָ��firstedge
	int networkCount;
	//int networkID[MAX_NETWORKS];
	RoutingTableEntry routingTable[MAX_NETWORKS];
} Router;

typedef struct Network {
	Router routers[MAX_ROUTERS];
	int routerCount;
} Network;

/*********
* �㷨���
* ��ʼ�������������·���������ó�ʼ��·�ɱ�
* ����·�ɱ������ھ�·�������͵�·�ɸ�����Ϣ�����ݽ��յ�����Ϣ���±���·�ɱ�
* �������£�����һ���Ĳ��ԣ�����ʱ�������¼�����������·��������·�ɸ�����Ϣ���ھ�·������
* ѡ�����·�ɣ�����·�ɱ��еľ�����Ϣѡ����ѵ���һ��·������
*********/

/*
* ��ʼ�������������·���������ó�ʼ��·�ɱ����ֶ�����+��ȡ�ļ���
*/

void initNetwork(Network* network) {    // ��ʼ��·�ɱ�
    network->routerCount = 0;
}

void addRouter(Network* network, int routerID) {    // ���·����
    if (network->routerCount < MAX_ROUTERS) {
        Router* router = &network->routers[network->routerCount];
        router->routerID = routerID;
        router->neighbors = NULL;
        network->routerCount++;
    }
}

void addNeighbor(Router* router, int neighborID, int distance) {    // ����ھ�
    Neighbor* neighbor = (Neighbor*)malloc(sizeof(Neighbor));
    neighbor->neighborID = neighborID;
    neighbor->distance = distance;
    neighbor->next = router->neighbors;
    router->neighbors = neighbor;
}

/*
* ����·�ɱ�
*/

void updateRoutingTable(Router* router, int networkID, int nextHop, int distance) { 
    int i;
    // ����·�ɱ����Ƿ��Ѵ���Ŀ���������Ŀ
    for (i = 0; i < MAX_NETWORKS; i++) {
        if (router->routingTable[i].networkID == networkID) {   // ����ҵ�Ŀ��������Ŀ�����¾������һ��
            router->routingTable[i].nextHop = nextHop; 
            router->routingTable[i].distance = distance;
            return;
        }
    }
    // ���·�ɱ��в�����Ŀ���������Ŀ���������Ŀ
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
* ����·����Ϣ����
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
* ����·�ɸ�����Ϣ���ھ�·����
*/

void triggerUpdate(Network* network, int routerID) { 
    Router* router = NULL;
    int i;

    // ����ָ��·����ID��·����
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

    // �����ھ��б���ÿ���ھӷ���·�ɸ�����Ϣ
    Neighbor* neighbor = router->neighbors;
    while (neighbor != NULL) {
        int neighborID = neighbor->neighborID;
        int networkID, distance; // ·�ɸ�����Ϣ��Ŀ������ID�;���

        // ����·�ɱ��е���Ϣ����·�ɸ�����Ϣ
        // ...

        // ����·�ɸ�����Ϣ���ھ�·����
        // ...

        neighbor = neighbor->next;
    }
}

int selectBestRoute(Router* router, int networkID) {    //ѡ�����·��
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

    // ���·����
    addRouter(&network, 1);
    addRouter(&network, 2);

    // ����ھ�
    addNeighbor(&network.routers[0], 2, 1);
    addNeighbor(&network.routers[1], 1, 1);

    // ����·�ɸ���
    processRoutingUpdate(&network.routers[0], 2, 10, 1);

    // ѡ�����·��
    int nextHop = selectBestRoute(&network.routers[0], 10);
    printf("Next hop for network 10: %d\n", nextHop);

    return 0;
}