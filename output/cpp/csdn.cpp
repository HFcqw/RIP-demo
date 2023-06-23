/*
 * Change Logs:
 * Date           Author       Notes
 * 2021-08.03     tyustli      first version
 */
 /*
  * Change Logs:
  * Date           Author       Notes
  * 2021-08.03     tyustli      first version
  */

#include <string.h>
#include <ctype.h>
#include <malloc.h> // malloc()��
#include <limits.h> // INT_MAX��
#include <stdio.h>  // EOF(=^Z��F6),NULL
#include <stdlib.h> // atoi()

  // �������״̬����
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;  // Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK��
typedef int Boolean; // Boolean�ǲ�������,��ֵ��TRUE��FALSE

#define MAX_VERTEX_NUM 20
#define MAX_NAME 3 // �����ַ�������󳤶�+1

typedef int InfoType;              // ����Ȩֵ����
typedef char VertexType[MAX_NAME]; // ��������Ϊ�ַ���

enum GraphKind
{
    DG,
    DN,
    UDG,
    UDN
}; // {����ͼ,������,����ͼ,������}

struct ElemType
{
    int adjvex;     // �û���ָ��Ķ����λ��
    InfoType* info; // ����Ȩֵָ��
};

struct ArcNode
{
    struct ElemType data;    // ��ָ������Ĳ��ֶ�����ElemType
    struct ArcNode* nextarc; // ָ����һ������ָ��
};                           // ����
typedef struct
{
    VertexType data;              // ������Ϣ
    struct ArcNode* firstarc;     // ��һ������ĵ�ַ,ָ���һ�������ö���Ļ���ָ��
} VNode, AdjList[MAX_VERTEX_NUM]; // ͷ���

struct ALGraph
{
    AdjList vertices;
    int vexnum, arcnum;  // ͼ�ĵ�ǰ�������ͻ���
    enum GraphKind kind; // ͼ�������־
};

#define LNode struct ArcNode      // ���嵥����Ľ��������ͼ�ı��������
#define next nextarc              // ���嵥�������ָ�����Ǳ���ָ����һ������ָ����
typedef struct ArcNode* LinkList; // ����ָ���������ָ����ָ��ͼ�ı����ָ��

void CreateGraph(struct ALGraph* G);
void CreateGraphF(struct ALGraph* G);
void Display(struct ALGraph G);
void DFSTraverse(struct ALGraph G, void (*Visit)(char*));
void BFSTraverse(struct ALGraph G, void (*Visit)(char*));

/************************ end of file **************************/


#include "graph.h"

#if 1
 // ����ͷ���ĵ�����Ĳ��ֻ������� �ڽӱ��л��õ�����Ĳ���Ȳ���
#define DestroyList ClearList // DestroyList()��ClearList()�Ĳ�����һ����
void InitList(LinkList* L)
{
    *L = NULL; // ָ��Ϊ��
}
void ClearList(LinkList* L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ������������L����Ϊ�ձ�
    LinkList p;
    while (*L) // L����
    {
        p = *L;          // pָ����Ԫ���
        *L = (*L)->next; // Lָ���2�����(����Ԫ���)
        free(p);         // �ͷ���Ԫ���
    }
}
Status ListEmpty(LinkList L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ������������LΪ�ձ��򷵻�TRUE�����򷵻�FALSE
    if (L)
        return FALSE;
    else
        return TRUE;
}
int ListLength(LinkList L)
{ // ��ʼ���������Ա�L�Ѵ��ڡ��������������L������Ԫ�ظ���
    int i = 0;
    LinkList p = L;
    while (p) // pָ����(û����β)
    {
        p = p->next; // pָ����һ�����
        i++;
    }
    return i;
}
Status GetElem(LinkList L, int i, struct ElemType* e)
{
    int j = 1;
    LinkList p = L;
    if (i < 1) // iֵ���Ϸ�
        return ERROR;
    while (j < i && p) // û����i��Ԫ�أ�Ҳû����β
    {
        j++;
        p = p->next;
    }
    if (j == i) // ���ڵ�i��Ԫ��
    {
        *e = p->data;
        return OK;
    }
    else
        return ERROR;
}
int LocateElem(LinkList L, struct ElemType e, Status(*compare)(struct ElemType, struct ElemType))
{
    int i = 0;
    LinkList p = L;
    while (p)
    {
        i++;
        if (compare(p->data, e)) // �ҵ�����������Ԫ��
            return i;
        p = p->next;
    }
    return 0;
}

Status ListInsert(LinkList* L, int i, struct ElemType e)
{ // �ڲ���ͷ���ĵ������Ա�L�е�i��λ��֮ǰ����Ԫ��e
    int j = 1;
    LinkList p = *L, s;
    if (i < 1) // iֵ���Ϸ�
        return ERROR;
    s = (LinkList)malloc(sizeof(LNode)); // �����½��
    s->data = e;                         // ��s��data��ֵ
    if (i == 1)                          // ���ڱ�ͷ
    {
        s->next = *L;
        *L = s; // �ı�L
    }
    else
    {                          // ���ڱ�����ദ
        while (p && j < i - 1) // Ѱ�ҵ�i-1�����
        {
            p = p->next;
            j++;
        }
        if (!p) // i���ڱ�+1
            return ERROR;
        s->next = p->next;
        p->next = s;
    }
    return OK;
}
Status ListDelete(LinkList* L, int i, struct ElemType* e)
{ // �ڲ���ͷ���ĵ������Ա�L�У�ɾ����i��Ԫ�أ�����e������ֵ
    int j = 1;
    LinkList p = *L, q;
    if (i == 1) // ɾ����1�����
    {
        *L = p->next; // L�ɵ�2����㿪ʼ
        *e = p->data;
        free(p); // ɾ�����ͷŵ�1�����
    }
    else
    {
        while (p->next && j < i - 1) // Ѱ�ҵ�i����㣬����pָ����ǰ��
        {
            p = p->next;
            j++;
        }
        if (!p->next || j > i - 1) // ɾ��λ�ò�����
            return ERROR;
        q = p->next; // ɾ�����ͷŽ��
        p->next = q->next;
        *e = q->data;
        free(q);
    }
    return OK;
}

LinkList Point(LinkList L, struct ElemType e, Status(*equal)(struct ElemType, struct ElemType), LinkList* p)
{ // ���ұ�L�����������Ľ�㡣���ҵ�������ָ��ý���ָ�룬pָ��ý���ǰ��(���ý����
    // ��Ԫ��㣬��p=NULL)�����L�������������Ľ�㣬�򷵻�NULL��p�޶��塣
    // ����equal()�����βεĹؼ�����ȣ�����OK�����򷵻�ERROR
    int i, j;
    i = LocateElem(L, e, equal);
    if (i) // �ҵ�
    {
        if (i == 1) // ����Ԫ���
        {
            *p = NULL;
            return L;
        }
        *p = L;
        for (j = 2; j < i; j++)
            *p = (*p)->next;
        return (*p)->next;
    }
    return NULL; // û�ҵ�
}

Status DeleteElem(LinkList* L, struct ElemType* e, Status(*equal)(struct ElemType, struct ElemType))
{ // ɾ����L�����������Ľ�㣬������TRUE�����޴˽�㣬�򷵻�FALSE��
    // ����equal()�����βεĹؼ�����ȣ�����OK�����򷵻�ERROR
    LinkList p, q;
    q = Point(*L, *e, equal, &p);
    if (q) // �ҵ��˽�㣬��qָ��ý��
    {
        if (p)                    // �ý�㲻����Ԫ��㣬pָ����ǰ��
            ListDelete(&p, 2, e); // ��p��Ϊͷָ��,ɾ����2�����
        else                      // �ý������Ԫ���
            ListDelete(L, 1, e);
        return TRUE;
    }
    return FALSE;
}

void ListTraverse(LinkList L, void (*vi)(struct ElemType))
{ // ��ʼ���������Ա�L�Ѵ��ڡ�������������ζ�L��ÿ������Ԫ�ص��ú���vi()
    LinkList p = L;
    while (p)
    {
        vi(p->data);
        p = p->next;
    }
    printf("\n");
}
#endif

// ͼ���ڽӱ�洢�Ļ�������

// ��ʼ������ͼ G ���ڣ�u �� G �ж�������ͬ����
// ����������� G �д��ڶ��� u���򷵻ظö�����ͼ��λ�ã����򷵻� -1
int LocateVex(struct ALGraph G, VertexType u)
{
    int i;
    for (i = 0; i < G.vexnum; ++i)
        if (strcmp(u, G.vertices[i].data) == 0)
            return i;
    return -1;
}

// �����ڽӱ�洢�ṹ������û�������Ϣͼ����G(��һ����������4��ͼ)

void CreateGraph(struct ALGraph* G)
{                      // �����ڽӱ�洢�ṹ������û�������Ϣͼ����G(��һ����������4��ͼ)
    int i, j, k, w;    // w��Ȩֵ
    VertexType va, vb; // ���ӱ߻򻡵�2����
    struct ElemType e;
    printf("������ͼ������(����ͼ:0,������:1,����ͼ:2,������:3): ");
    scanf("%d", (int*)&G->kind);
    printf("������ͼ�Ķ�����,����: ");
    scanf("%d,%d", &G->vexnum, &G->arcnum);
    printf("������%d�������ֵ(<%d���ַ�):\n", G->vexnum, MAX_NAME);
    for (i = 0; i < G->vexnum; ++i) // ���춥������
    {
        scanf("%s", G->vertices[i].data);
        G->vertices[i].firstarc = NULL; // ��ʼ����ö����йصĳ�������
    }
    if (G->kind % 2) // ��
        printf("������ÿ����(��)��Ȩֵ����β�ͻ�ͷ(�Կո���Ϊ���):\n");
    else // ͼ
        printf("������ÿ����(��)�Ļ�β�ͻ�ͷ(�Կո���Ϊ���):\n");
    for (k = 0; k < G->arcnum; ++k) // ������ػ�����
    {
        if (G->kind % 2) // ��
            scanf("%d%s%s", &w, va, vb);
        else // ͼ
            scanf("%s%s", va, vb);
        i = LocateVex(*G, va); // ��β
        j = LocateVex(*G, vb); // ��ͷ
        e.info = NULL;         // ���������e��ֵ��ͼ��Ȩ
        e.adjvex = j;          // ��ͷ
        if (G->kind % 2)       // ��
        {
            e.info = (int*)malloc(sizeof(int)); // ��̬���ɴ��Ȩֵ�Ŀռ�
            *(e.info) = w;
        }
        ListInsert(&G->vertices[i].firstarc, 1, e); // ���ڵ�i��Ԫ��(����)�ı�ͷ����bo2-8.cpp��
        if (G->kind >= 2)                           // ����ͼ������������2�����㣬�����ڵ�j��Ԫ��(�뻡)�ı�ͷ
        {
            e.adjvex = i;                               // e.info���䣬�����ٸ�ֵ
            ListInsert(&G->vertices[j].firstarc, 1, e); // ���ڵ�j��Ԫ�صı�ͷ����bo2-8.cpp��
        }
    }
}

// �����ڽӱ�洢�ṹ�����ļ�����û�������Ϣͼ����G(��һ����������4��ͼ)
void CreateGraphF(struct ALGraph* G)
{
    int i, j, k, w;    // w��Ȩֵ
    VertexType va, vb; // ���ӱ߻򻡵�2����
    struct ElemType e;
    char filename[13];
    FILE* graphlist;
    printf("�����������ļ���(f7-1.txt��f7-2.txt)��");
    scanf("%s", filename);
    printf("������ͼ������(����ͼ:0,������:1,����ͼ:2,������:3): ");
    scanf("%d", (int*)&G->kind);
    graphlist = fopen(filename, "r"); // �Զ��ķ�ʽ�������ļ�������graphlist��ʾ
    fscanf(graphlist, "%d", &G->vexnum);
    fscanf(graphlist, "%d", &G->arcnum);
    for (i = 0; i < G->vexnum; ++i) // ���춥������
    {
        fscanf(graphlist, "%s", G->vertices[i].data);
        G->vertices[i].firstarc = NULL; // ��ʼ����ö����йصĳ�������
    }
    for (k = 0; k < G->arcnum; ++k) // ������ػ�����
    {
        if (G->kind % 2) // ��
            fscanf(graphlist, "%d%s%s", &w, va, vb);
        else // ͼ
            fscanf(graphlist, "%s%s", va, vb);
        i = LocateVex(*G, va); // ��β
        j = LocateVex(*G, vb); // ��ͷ
        e.info = NULL;         // ���������e��ֵ��ͼ��Ȩ
        e.adjvex = j;          // ��ͷ
        if (G->kind % 2)       // ��
        {
            e.info = (int*)malloc(sizeof(int)); // ��̬���ɴ��Ȩֵ�Ŀռ�
            *(e.info) = w;
        }
        ListInsert(&G->vertices[i].firstarc, 1, e); // ���ڵ�i��Ԫ��(����)�ı�ͷ����bo2-8.cpp��
        if (G->kind >= 2)                           // ����ͼ������������2�����㣬�����ڵ�j��Ԫ��(�뻡)�ı�ͷ
        {
            e.adjvex = i;                               // e.info���䣬�����ٸ�ֵ
            ListInsert(&G->vertices[j].firstarc, 1, e); // ���ڵ�j��Ԫ�صı�ͷ����bo2-8.cpp��
        }
    }
    fclose(graphlist); // �ر������ļ�
}

void DestroyGraph(struct ALGraph* G)
{ // ��ʼ������ͼG���ڡ��������������ͼG
    int i;
    struct ElemType e;
    for (i = 0; i < G->vexnum; ++i)         // �������ж���
        if (G->kind % 2)                    // ��
            while (G->vertices[i].firstarc) // ��Ӧ�Ļ����������
            {
                ListDelete(&G->vertices[i].firstarc, 1, &e); // ɾ������ĵ�1����㣬����ֵ����e
                if (e.adjvex > i)                            // �������>i(��֤��̬���ɵ�Ȩֵ�ռ�ֻ�ͷ�1��)
                    free(e.info);
            }
        else                                       // ͼ
            DestroyList(&G->vertices[i].firstarc); // ���ٻ����������bo2-8.cpp��
    G->vexnum = 0;                                 // ������Ϊ0
    G->arcnum = 0;                                 // �߻���Ϊ0
}

// ��ʼ������ͼG���ڣ�v��G��ĳ���������š��������������v��ֵ
VertexType* GetVex(struct ALGraph G, int v)
{
    if (v >= G.vexnum || v < 0)
        exit(ERROR);
    return G.vertices[v].data;
}

// ��ʼ������ͼG���ڣ�v��G��ĳ�����㡣�����������v����ֵvalue
Status PutVex(struct ALGraph* G, VertexType v, VertexType value)
{
    int i;
    i = LocateVex(*G, v);
    if (i > -1) // v��G�Ķ���
    {
        strcpy(G->vertices[i].data, value);
        return OK;
    }
    return ERROR;
}

// ��ʼ������ͼG���ڣ�v��G��ĳ������
// �������������v�ĵ�һ���ڽӶ������š���������G��û���ڽӶ��㣬�򷵻�-1
int FirstAdjVex(struct ALGraph G, VertexType v)
{
    struct ArcNode* p;
    int v1;
    v1 = LocateVex(G, v); // v1Ϊ����v��ͼG�е����
    p = G.vertices[v1].firstarc;
    if (p)
        return p->data.adjvex;
    else
        return -1;
}

// DeleteArc()��DeleteVex()��NextAdjVex()Ҫ���õĺ���
Status equalvex(struct ElemType a, struct ElemType b)
{
    if (a.adjvex == b.adjvex)
        return OK;
    else
        return ERROR;
}

// ��ʼ������ͼG���ڣ�v��G��ĳ�����㣬w��v���ڽӶ���
// �������������v��(�����w��)��һ���ڽӶ������š���w��v�����һ���ڽӵ㣬�򷵻�-1
int NextAdjVex(struct ALGraph G, VertexType v, VertexType w)
{
    LinkList p, p1; // p1��Point()����������ָ�룬Point()��func2-1.cpp��
    struct ElemType e;
    int v1;
    v1 = LocateVex(G, v);                                 // v1Ϊ����v��ͼG�е����
    e.adjvex = LocateVex(G, w);                           // e.adjvexΪ����w��ͼG�е����
    p = Point(G.vertices[v1].firstarc, e, equalvex, &p1); // pָ�򶥵�v���������ڽӶ���Ϊw�Ľ��
    if (!p || !p->next)                                   // û�ҵ�w��w�����һ���ڽӵ�
        return -1;
    else                             // p->data.adjvex==w
        return p->next->data.adjvex; // ����v��(�����w��)��һ���ڽӶ�������
}

// ��ʼ������ͼG���ڣ�v��ͼ�ж�������ͬ����
// �����������ͼG�������¶���v(�������붥����صĻ�������InsertArc()ȥ��)
void InsertVex(struct ALGraph* G, VertexType v)
{
    strcpy(G->vertices[G->vexnum].data, v); // �����¶�������
    G->vertices[G->vexnum].firstarc = NULL;
    G->vexnum++; // ͼG�Ķ�������1
}

// ��ʼ������ͼG���ڣ�v��G��ĳ�����㡣���������ɾ��G�ж���v������صĻ�
Status DeleteVex(struct ALGraph* G, VertexType v)
{
    int i, j, k;
    struct ElemType e;
    LinkList p, p1;
    j = LocateVex(*G, v); // j�Ƕ���v�����
    if (j < 0)            // v����ͼG�Ķ���
        return ERROR;
    i = ListLength(G->vertices[j].firstarc); // ��vΪ���ȵĻ����������bo2-8.cpp��
    G->arcnum -= i;                          // �߻���-i
    if (G->kind % 2)                         // ��
        while (G->vertices[j].firstarc)      // ��Ӧ�Ļ����������
        {
            ListDelete(&G->vertices[j].firstarc, 1, &e); // ɾ������ĵ�1����㣬����ֵ����e
            free(e.info);                                // �ͷŶ�̬���ɵ�Ȩֵ�ռ�
        }
    else                                       // ͼ
        DestroyList(&G->vertices[j].firstarc); // ���ٻ����������bo2-8.cpp��
    G->vexnum--;                               // ��������1
    for (i = j; i < G->vexnum; i++)            // ����v����Ķ���ǰ��
        G->vertices[i] = G->vertices[i + 1];
    for (i = 0; i < G->vexnum; i++) // ɾ����vΪ��ȵĻ�����ұ�Ҫʱ�޸ı���Ķ���λ��ֵ
    {
        e.adjvex = j;
        p = Point(G->vertices[i].firstarc, e, equalvex, &p1); // Point()��func2-1.cpp��
        if (p)                                                // ����i���ڽӱ�����vΪ��ȵĽ��
        {
            if (p1)                                // p1ָ��p��ָ����ǰ��
                p1->next = p->next;                // ��������ɾ��p��ָ���
            else                                   // pָ����Ԫ���
                G->vertices[i].firstarc = p->next; // ͷָ��ָ����һ���
            if (G->kind < 2)                       // ����
            {
                G->arcnum--;            // �߻���-1
                if (G->kind == 1)       // ������
                    free(p->data.info); // �ͷŶ�̬���ɵ�Ȩֵ�ռ�
            }
            free(p); // �ͷ�vΪ��ȵĽ��
        }
        for (k = j + 1; k <= G->vexnum; k++) // ����adjvex��>j�Ľ�㣬�����-1
        {
            e.adjvex = k;
            p = Point(G->vertices[i].firstarc, e, equalvex, &p1); // Point()��func2-1.cpp��
            if (p)
                p->data.adjvex--; // ���-1(��Ϊǰ��)
        }
    }
    return OK;
}

// ��ʼ������ͼG���ڣ�v��w��G����������
// �����������G������<v,w>����G������ģ�������Գƻ�<w,v>
Status InsertArc(struct ALGraph* G, VertexType v, VertexType w)
{
    struct ElemType e;
    int i, j;
    i = LocateVex(*G, v); // ��β��ߵ����
    j = LocateVex(*G, w); // ��ͷ��ߵ����
    if (i < 0 || j < 0)
        return ERROR;
    G->arcnum++; // ͼG�Ļ���ߵ���Ŀ��1
    e.adjvex = j;
    e.info = NULL;   // ��ֵ
    if (G->kind % 2) // ��
    {
        e.info = (int*)malloc(sizeof(int)); // ��̬���ɴ��Ȩֵ�Ŀռ�
        printf("�����뻡(��)%s��%s��Ȩֵ: ", v, w);
        scanf("%d", e.info);
    }
    ListInsert(&G->vertices[i].firstarc, 1, e); // ��e���ڻ�β�ı�ͷ����bo2-8.cpp��
    if (G->kind >= 2)                           // ����������һ������
    {
        e.adjvex = i;                               // e.info����
        ListInsert(&G->vertices[j].firstarc, 1, e); // ��e���ڻ�ͷ�ı�ͷ
    }
    return OK;
}

// ��ʼ������ͼG���ڣ�v��w��G����������
// �����������G��ɾ����<v,w>����G������ģ���ɾ���Գƻ�<w,v>
Status DeleteArc(struct ALGraph* G, VertexType v, VertexType w)
{
    int i, j;
    Status k;
    struct ElemType e;
    i = LocateVex(*G, v); // i�Ƕ���v(��β)�����
    j = LocateVex(*G, w); // j�Ƕ���w(��ͷ)�����
    if (i < 0 || j < 0 || i == j)
        return ERROR;
    e.adjvex = j;
    k = DeleteElem(&G->vertices[i].firstarc, &e, equalvex); // ��func2-1.cpp��
    if (k)                                                  // ɾ���ɹ�
    {
        G->arcnum--;     // ���������1
        if (G->kind % 2) // ��
            free(e.info);
        if (G->kind >= 2) // ����ɾ���Գƻ�<w,v>
        {
            e.adjvex = i;
            DeleteElem(&G->vertices[j].firstarc, &e, equalvex);
        }
        return OK;
    }
    else // û�ҵ���ɾ���Ļ�
    {
        return ERROR;
    }
}

Boolean visited[MAX_VERTEX_NUM]; // ���ʱ�־����(ȫ����)
void (*VisitFunc)(char* v);      // ��������(ȫ����)
// �ӵ�v����������ݹ��������ȱ���ͼG���㷨7.5
void DFS(struct ALGraph G, int v)
{
    int w;
    visited[v] = TRUE;             // ���÷��ʱ�־ΪTRUE(�ѷ���)
    VisitFunc(G.vertices[v].data); // ���ʵ�v������
    for (w = FirstAdjVex(G, G.vertices[v].data); w >= 0; w = NextAdjVex(G, G.vertices[v].data, G.vertices[w].data))
        if (!visited[w])
            DFS(G, w); // ��v����δ���ʵ��ڽӵ�w�ݹ����DFS
}

// ��ͼG��������ȱ������㷨7.4
void DFSTraverse(struct ALGraph G, void (*Visit)(char*))
{
    int v;
    VisitFunc = Visit; // ʹ��ȫ�ֱ���VisitFunc��ʹDFS�����躯��ָ�����
    for (v = 0; v < G.vexnum; v++)
        visited[v] = FALSE; // ���ʱ�־�����ʼ��
    for (v = 0; v < G.vexnum; v++)
        if (!visited[v])
            DFS(G, v); // ����δ���ʵĶ������DFS
    printf("\n");
}

typedef int QElemType; // ����Ԫ������

#if 1
typedef struct QNode
{
    QElemType data;     /* ������ */
    struct QNode* next; /* ָ���� */
} QNode, * QueuePtr;
typedef struct LinkQueue
{
    QueuePtr front; // ��ͷָ��
    QueuePtr rear;  // ��βָ��
} LinkQueue;
void InitQueue(LinkQueue* Q);
void DestroyQueue(LinkQueue* Q);
void ClearQueue(LinkQueue* Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead(LinkQueue Q, QElemType* e);
void EnQueue(LinkQueue* Q, QElemType e);
Status DeQueue(LinkQueue* Q, QElemType* e);
void QueueTraverse(LinkQueue Q, void (*vi)(QElemType));
void print(QElemType i);
void InitQueue(LinkQueue* Q)
{                                                         // ����һ���ն��� Q
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode)); /* �������ͷ��� */

    if (Q->front == NULL)
        exit(-1);
    Q->front->next = NULL;
}
void DestroyQueue(LinkQueue* Q)
{
    while (Q->front)
    {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;
    }
}
void ClearQueue(LinkQueue* Q)
{
    QueuePtr p, q;
    Q->rear = Q->front;
    p = Q->front->next;
    Q->front->next = NULL;
    while (p)
    {
        q = p;
        p = p->next;
        free(q);
    }
}
Status QueueEmpty(LinkQueue Q)
{
    if (Q.front->next == NULL)
        return TRUE;
    else
        return FALSE;
}
int QueueLength(LinkQueue Q)
{
    int i = 0;
    QueuePtr p;
    p = Q.front;
    while (Q.rear != p)
    {
        i++;
        p = p->next;
    }
    return i;
}
Status GetHead(LinkQueue Q, QElemType* e)
{
    QueuePtr p;
    if (Q.front == Q.rear)
        return ERROR;
    p = Q.front->next;
    *e = p->data;

    return OK;
}
void EnQueue(LinkQueue* Q, QElemType e)
{
    QueuePtr p;
    if (!(p = (QueuePtr)malloc(sizeof(QNode)))) // �洢����ʧ��
        exit(-1);
    p->data = e;
    p->next = NULL;    /* �½��� next Ϊ�� */
    Q->rear->next = p; /* ��һ�ε�βָ��ָ���µĽ�� */
    Q->rear = p;       /* �µ�βָ�� */
}
Status DeQueue(LinkQueue* Q, QElemType* e)
{
    QueuePtr p;
    if (Q->front == Q->rear)
        return ERROR;
    p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)
        Q->rear = Q->front;
    free(p);

    return OK;
}
void QueueTraverse(LinkQueue Q, void (*vi)(QElemType))
{
    QueuePtr p;
    p = Q.front->next;
    while (p)
    {
        vi(p->data);
        p = p->next;
    }
    printf("\n");
}

void print(QElemType i)
{
    // printf("%s ", i);
}
#endif

//��������ȷǵݹ����ͼG��ʹ�ø�������Q�ͷ��ʱ�־����visited���㷨7.6
void BFSTraverse(struct ALGraph G, void (*Visit)(char*))
{
    int v, u, w;
    LinkQueue Q;
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = FALSE;        // �ó�ֵ
    InitQueue(&Q);                 // �ÿյĸ�������Q
    for (v = 0; v < G.vexnum; v++) // �������ͨͼ��ֻv=0�ͱ���ȫͼ
        if (!visited[v])           // v��δ����
        {
            visited[v] = TRUE;
            Visit(G.vertices[v].data);
            EnQueue(&Q, v);        // v�����
            while (!QueueEmpty(Q)) // ���в���
            {
                DeQueue(&Q, &u); // ��ͷԪ�س��Ӳ���Ϊu
                for (w = FirstAdjVex(G, G.vertices[u].data); w >= 0; w = NextAdjVex(G, G.vertices[u].data, G.vertices[w].data))
                    if (!visited[w]) // wΪu����δ���ʵ��ڽӶ���
                    {
                        visited[w] = TRUE;
                        Visit(G.vertices[w].data);
                        EnQueue(&Q, w); // w���
                    }
            }
        }
    printf("\n");
}

// �ӵ�v����������ݹ��������ȱ���ͼG�����������ڽӱ�洢�ṹ
void DFS1(struct ALGraph G, int v, void (*Visit)(char*))
{
    struct ArcNode* p;                               // pָ�����
    visited[v] = TRUE;                               // ���÷��ʱ�־ΪTRUE(�ѷ���)
    Visit(G.vertices[v].data);                       // ���ʸö���
    for (p = G.vertices[v].firstarc; p; p = p->next) // p����ָ��v���ڽӶ���
        if (!visited[p->data.adjvex])
            DFS1(G, p->data.adjvex, Visit); // ��v����δ���ʵ��ڽӵ�ݹ����DFS1
}

// ��ͼG��������ȱ�����DFS1�躯��ָ�����
void DFSTraverse1(struct ALGraph G, void (*Visit)(char*))
{
    int v;
    for (v = 0; v < G.vexnum; v++)
        visited[v] = FALSE;        // ���ʱ�־�����ʼ�����ó�ֵΪδ������
    for (v = 0; v < G.vexnum; v++) // �������ͨͼ��ֻv=0�ͱ���ȫͼ
        if (!visited[v])           // v��δ������
            DFS1(G, v, Visit);     // ��v����DFS1
    printf("\n");
}

// ��������ȷǵݹ����ͼG��ʹ�ø�������Q�ͷ��ʱ�־����visited�����������ڽӱ�ṹ
void BFSTraverse1(struct ALGraph G, void (*Visit)(char*))
{
    int v, u;
    struct ArcNode* p; // pָ�����
    LinkQueue Q;       // ����������
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = FALSE;        // �ó�ֵΪδ������
    InitQueue(&Q);                 // ��ʼ����������Q
    for (v = 0; v < G.vexnum; v++) // �������ͨͼ��ֻv=0�ͱ���ȫͼ
        if (!visited[v])           // v��δ������
        {
            visited[v] = TRUE;         // ��vΪ�ѱ�����
            Visit(G.vertices[v].data); // ����v
            EnQueue(&Q, v);            // v�����
            while (!QueueEmpty(Q))     // ���в���
            {
                DeQueue(&Q, &u);                                 // ��ͷԪ�س��Ӳ���Ϊu
                for (p = G.vertices[u].firstarc; p; p = p->next) // p����ָ��u���ڽӶ���
                    if (!visited[p->data.adjvex])                // u���ڽӶ�����δ������
                    {
                        visited[p->data.adjvex] = TRUE;         // ���ڽӶ�����Ϊ�ѱ�����
                        Visit(G.vertices[p->data.adjvex].data); // ���ʸ��ڽӶ���
                        EnQueue(&Q, p->data.adjvex);            // ��Ӹ��ڽӶ������
                    }
            }
        }
    printf("\n");
}

// ���ͼ���ڽӾ���G
void Display(struct ALGraph G)
{
    int i;
    struct ArcNode* p;
    switch (G.kind)
    {
    case DG:
        printf("����ͼ\n");
        break;
    case DN:
        printf("������\n");
        break;
    case UDG:
        printf("����ͼ\n");
        break;
    case UDN:
        printf("������\n");
    }
    printf("%d�����㣺\n", G.vexnum);
    for (i = 0; i < G.vexnum; ++i)
        printf("%s ", G.vertices[i].data);
    printf("\n%d����(��):\n", G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        p = G.vertices[i].firstarc;
        while (p)
        {
            if (G.kind <= 1 || i < p->data.adjvex) // ��������������е�һ��
            {
                printf("%s��%s ", G.vertices[i].data, G.vertices[p->data.adjvex].data);
                if (G.kind % 2) // ��
                    printf(":%d ", *(p->data.info));
            }
            p = p->nextarc;
        }
        printf("\n");
    }
}

/************************ end of file **************************/

