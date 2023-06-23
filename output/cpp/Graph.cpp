/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_ROUTERS 26
#define MAX_NETWORKS 100
#define INFINITY 16

#define MAXVEX 100	//ͼ�ж�����Ŀ�����ֵ
typedef char VertexType;	//��������
typedef int EdgeType;	//���ϵ�Ȩֵ����

//�߱���
typedef struct EdgeNode {
	int adjvex;	//�ڽӵ��򣬴洢�ö����Ӧ���±�
	EdgeType weight;	//Ȩֵ�����ڷ���ͼ���Բ���Ҫ
	struct EdgeNode* next;	//ָ����һ���ڽӵ�
}EdgeNode;
//�������
typedef struct VertexNode {
	VertexType data;	//�����򣬴洢������Ϣ
	EdgeNode* firstedge;	//�߱�ͷָ��
} VertexNode, AdjList[MAXVEX];
//�ڽӱ�
typedef struct GraphAdjList {
	AdjList adjList;
	int numVertexes, numEdges;	//ͼ�е�ǰ�������ͱ���
}GraphAdjList;

typedef struct RoutTable { //����·�ɱ� 
	unsigned char tn; 	   //Ŀ�������ַ 
	unsigned char d; 	   //���� 
	char next;			   //��һ��·������
}RTable;
typedef struct Router {
	int routerId;
	RTable routerTable;
	int* nearRouter;
	int* nearNetwork;
}Router[MAX_ROUTERS];
typedef struct Network {
	int routernum;
	int networknum;
	Router routers;
}Network;

void CreateALGragh(GraphAdjList* G) {
	int i, j, k;
	EdgeNode* e;
	printf("���붥�����ͱ�����\n");
	scanf_s("%d %d", &G->numVertexes, &G->numEdges);
	for (i = 0; i < G->numVertexes; i++) {
		scanf_s(&G->adjList[i].data);
		G->adjList[i].firstedge = NULL;
	}
	for (k = 0; k < G->numEdges; k++) {
		printf("����ߣ�Vi,Vj���ϵĶ������i,j��\n");
		scanf_s("%d %d", &i, &j);
		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = j;
		e->next = G->adjList[i].firstedge;
		G->adjList[i].firstedge = e;
		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = i;
		e->next = G->adjList[j].firstedge;
		G->adjList[j].firstedge = e;
	}
}
int main() {
	GraphAdjList G;
	CreateALGragh(&G);
	return 0;
}
*/