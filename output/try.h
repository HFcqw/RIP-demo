#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <malloc.h>
#include <time.h>
#include<assert.h>

// �������״̬����
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define MAX_ROUTERS 20	//��RIP������·���������Ϊ20
#define MAX_NETWORKS 20	//�����������Ϊ10
#define MAX_ENTRIES 25	//һ��RIP�������ɰ���25��·��
#define INFINITY 16	//һ��ͨ·�����ɰ�����·����������15��,16����ֹ

typedef int Status;  // Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK��
typedef int Boolean; // Boolean�ǲ�������,��ֵ��TRUE��FALSE
typedef char RouterType;	//�������ͣ�·�������֣�
typedef char NetworkType;
typedef int EdgeType;	//���ϵ�Ȩֵ���ͣ�RIP����̶�Ϊ1��

typedef struct RoutingTable {	//·�ɱ�ṹ�� 
	char networkID;	//Ŀ�������ַ����������ţ� 
	char nextHop;	//��һ�� 
	int distance;	//����
} RTable;

typedef struct Neighbor {	//�߱��㣨�ڽ�·������
	int neighborID;	//�ڽӵ��򣬴洢�ö����Ӧ���±�
	EdgeType distance;	//���ϵ�Ȩֵ���̶�Ϊ1
	struct Neighbor* next;	//ָ����һ���ڽӵ�/��һ���� ��ָ��
} Neighbor, * LinkList;

typedef struct Router { //������㣨�ڵ������·������������ֱ�����磩
	RouterType routerID;
	int neighborCount;
	int dirnetworkCount;	//�м���ֱ������
	NetworkType dirNetworkID[MAX_NETWORKS];	//��·����ֱ��������
	Neighbor* neighbors;	//�߱�ͷָ��firstedge��ָ���һ�������ڸö���Ļ���ָ��
	RTable* routingTable;	//·�ɱ�����
} Router, RouterList[MAX_ROUTERS];

typedef struct Network {	//����·������ɵ�ͼ������ֱ��������Ϊ�ڵ�
	RouterList RL;  //���·��������˳���
	//DNList DL;	//���Ŀ��������Ϣ
	int routerCount;    //·��������
	int edgeCount;    //·����ֱ�����ɵ�ͼ�ıߵ�����
	int networkCount;	//���������
} Network;

/*
typedef struct DestNetwork {	//Ŀ������Ľṹ��
	NetworkType dnetworkID;
	int dirrouterCount;	//�м���ֱ��������
	RouterType routerID[MAX_ROUTERS];	//������ֱ����·����
	char* road;
	RTable routingTable;
}DNet, DNList[MAX_NETWORKS];
*/

/*********
********** ���ݽṹ���
* ·�ɱ�
* �ڽ�·���������߽�㣩
* ·�����������㣩,ÿ��������ĵ�һ��������йض������Ϣ���������㿴������ı�ͷ�����������йرߵ���Ϣ
* Ŀ������
* ȫ������
********** �㷨���
* * * ͼ�Ļ�������
* ͼ�Ĵ洢
* ��ӽڵ㡢�޸Ľڵ�������Ϣ��ɾ���ڵ�
* ��ӱߡ�ɾ����
* ����ͼ
* * * * RIP��������㷨
* ��ʼ�������������·����������ͼ�������ó�ʼ��·�ɱ�
* ����·�ɱ������ھ�·�������͵�·�ɸ�����Ϣ�����ݽ��յ�����Ϣ���±���·�ɱ�
* �������£�����һ���Ĳ��ԣ�����ʱ�������¼�����������·��������·�ɸ�����Ϣ���ھ�·����
* ѡ�����·�ɣ�����·�ɱ��еľ�����Ϣѡ����ѵ���һ��·����
* ��ӡ·�ɱ�
*********/


/*
������ݽṹ��

�����ʾ·�����Ľṹ�壬����·����ID���ھ��б��·�ɱ����Ϣ��
�����ʾ�ھ�·�����Ľṹ�壬�����ھ�ID���ھӽӿڵ���Ϣ��
�����ʾ·�ɱ���Ľṹ�壬����Ŀ������ID����һ��·����ID������͸���ʱ�����Ϣ��
�����ʾ����Ľṹ�壬����·�����б������ID����Ϣ��

ʵ�ִ��ļ���ȡ�������ã�
��ƺ��������ļ��ж�ȡ����������Ϣ�������ݶ�ȡ������Ϣ�����������ݽṹ�������ļ����԰���·����ID���ھ��б�·�ɱ����Ϣ��

ʵ��·����Ϣ���º�ά���㷨��
��ƺ���������·����Ϣ�ĸ��º�ά�������Ը���RIPЭ��淶��ʹ�þ��������㷨����·�ɱ�
��·����Ϣ���¹����У������ھ��б�·�ɱ��·������״̬����Ϣ��

ʵ��·��ѡ���㷨��
��ƺ���������·�ɱ�ѡ�����·�ɡ����Ը��ݾ������������ѡ������·����
*/

/*
��׼�ĵ� RFC 1058
Ҫ������ϵͳ�ڵ�ÿһ��·������Ҫά�������Լ�������ϵͳ������ÿһ������ľ����¼����������Ϊ��������
RIPʹ����������������Ŀ������ľ���
·������ֱ������ľ��붨��Ϊ1��˼��·��������Ϊ0��
·��������ֱ������ľ��붨��Ϊ��������·��������1
һ��·�����ֻ�ܰ���15��·�����������롱����16ʱ�൱�ڲ��ɴ�
ѡ��·�����������ٵ�·��
�ȼ۸��ؾ���
·������������·����������Ϣ�������Լ���·�ɱ������Խ�����ÿ30�뷢��һ��RIP���±��ģ�
*/