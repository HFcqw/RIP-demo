/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_ROUTERS 26
#define MAX_NETWORKS 100
#define INFINITY 16

#define MAXVEX 100	//图中顶点数目的最大值
typedef char VertexType;	//顶点类型
typedef int EdgeType;	//边上的权值类型

//边表结点
typedef struct EdgeNode {
	int adjvex;	//邻接点域，存储该顶点对应的下标
	EdgeType weight;	//权值，对于非网图可以不需要
	struct EdgeNode* next;	//指向下一个邻接点
}EdgeNode;
//顶点表结点
typedef struct VertexNode {
	VertexType data;	//顶点域，存储顶点信息
	EdgeNode* firstedge;	//边表头指针
} VertexNode, AdjList[MAXVEX];
//邻接表
typedef struct GraphAdjList {
	AdjList adjList;
	int numVertexes, numEdges;	//图中当前顶点数和边数
}GraphAdjList;

typedef struct RoutTable { //定义路由表 
	unsigned char tn; 	   //目的网络地址 
	unsigned char d; 	   //距离 
	char next;			   //下一跳路由名字
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
	printf("输入顶点数和边数：\n");
	scanf_s("%d %d", &G->numVertexes, &G->numEdges);
	for (i = 0; i < G->numVertexes; i++) {
		scanf_s(&G->adjList[i].data);
		G->adjList[i].firstedge = NULL;
	}
	for (k = 0; k < G->numEdges; k++) {
		printf("输入边（Vi,Vj）上的顶点序号i,j：\n");
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