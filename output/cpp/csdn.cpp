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
#include <malloc.h> // malloc()等
#include <limits.h> // INT_MAX等
#include <stdio.h>  // EOF(=^Z或F6),NULL
#include <stdlib.h> // atoi()

  // 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;  // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int Boolean; // Boolean是布尔类型,其值是TRUE或FALSE

#define MAX_VERTEX_NUM 20
#define MAX_NAME 3 // 顶点字符串的最大长度+1

typedef int InfoType;              // 网的权值类型
typedef char VertexType[MAX_NAME]; // 顶点类型为字符串

enum GraphKind
{
    DG,
    DN,
    UDG,
    UDN
}; // {有向图,有向网,无向图,无向网}

struct ElemType
{
    int adjvex;     // 该弧所指向的顶点的位置
    InfoType* info; // 网的权值指针
};

struct ArcNode
{
    struct ElemType data;    // 除指针以外的部分都属于ElemType
    struct ArcNode* nextarc; // 指向下一条弧的指针
};                           // 表结点
typedef struct
{
    VertexType data;              // 顶点信息
    struct ArcNode* firstarc;     // 第一个表结点的地址,指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM]; // 头结点

struct ALGraph
{
    AdjList vertices;
    int vexnum, arcnum;  // 图的当前顶点数和弧数
    enum GraphKind kind; // 图的种类标志
};

#define LNode struct ArcNode      // 定义单链表的结点类型是图的表结点的类型
#define next nextarc              // 定义单链表结点的指针域是表结点指向下一条弧的指针域
typedef struct ArcNode* LinkList; // 定义指向单链表结点的指针是指向图的表结点的指针

void CreateGraph(struct ALGraph* G);
void CreateGraphF(struct ALGraph* G);
void Display(struct ALGraph G);
void DFSTraverse(struct ALGraph G, void (*Visit)(char*));
void BFSTraverse(struct ALGraph G, void (*Visit)(char*));

/************************ end of file **************************/


#include "graph.h"

#if 1
 // 不带头结点的单链表的部分基本操作 邻接表中会用到链表的插入等操作
#define DestroyList ClearList // DestroyList()和ClearList()的操作是一样的
void InitList(LinkList* L)
{
    *L = NULL; // 指针为空
}
void ClearList(LinkList* L)
{ // 初始条件：线性表L已存在。操作结果：将L重置为空表
    LinkList p;
    while (*L) // L不空
    {
        p = *L;          // p指向首元结点
        *L = (*L)->next; // L指向第2个结点(新首元结点)
        free(p);         // 释放首元结点
    }
}
Status ListEmpty(LinkList L)
{ // 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE
    if (L)
        return FALSE;
    else
        return TRUE;
}
int ListLength(LinkList L)
{ // 初始条件：线性表L已存在。操作结果：返回L中数据元素个数
    int i = 0;
    LinkList p = L;
    while (p) // p指向结点(没到表尾)
    {
        p = p->next; // p指向下一个结点
        i++;
    }
    return i;
}
Status GetElem(LinkList L, int i, struct ElemType* e)
{
    int j = 1;
    LinkList p = L;
    if (i < 1) // i值不合法
        return ERROR;
    while (j < i && p) // 没到第i个元素，也没到表尾
    {
        j++;
        p = p->next;
    }
    if (j == i) // 存在第i个元素
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
        if (compare(p->data, e)) // 找到这样的数据元素
            return i;
        p = p->next;
    }
    return 0;
}

Status ListInsert(LinkList* L, int i, struct ElemType e)
{ // 在不带头结点的单链线性表L中第i个位置之前插入元素e
    int j = 1;
    LinkList p = *L, s;
    if (i < 1) // i值不合法
        return ERROR;
    s = (LinkList)malloc(sizeof(LNode)); // 生成新结点
    s->data = e;                         // 给s的data域赋值
    if (i == 1)                          // 插在表头
    {
        s->next = *L;
        *L = s; // 改变L
    }
    else
    {                          // 插在表的其余处
        while (p && j < i - 1) // 寻找第i-1个结点
        {
            p = p->next;
            j++;
        }
        if (!p) // i大于表长+1
            return ERROR;
        s->next = p->next;
        p->next = s;
    }
    return OK;
}
Status ListDelete(LinkList* L, int i, struct ElemType* e)
{ // 在不带头结点的单链线性表L中，删除第i个元素，并由e返回其值
    int j = 1;
    LinkList p = *L, q;
    if (i == 1) // 删除第1个结点
    {
        *L = p->next; // L由第2个结点开始
        *e = p->data;
        free(p); // 删除并释放第1个结点
    }
    else
    {
        while (p->next && j < i - 1) // 寻找第i个结点，并令p指向其前趋
        {
            p = p->next;
            j++;
        }
        if (!p->next || j > i - 1) // 删除位置不合理
            return ERROR;
        q = p->next; // 删除并释放结点
        p->next = q->next;
        *e = q->data;
        free(q);
    }
    return OK;
}

LinkList Point(LinkList L, struct ElemType e, Status(*equal)(struct ElemType, struct ElemType), LinkList* p)
{ // 查找表L中满足条件的结点。如找到，返回指向该结点的指针，p指向该结点的前驱(若该结点是
    // 首元结点，则p=NULL)。如表L中无满足条件的结点，则返回NULL，p无定义。
    // 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR
    int i, j;
    i = LocateElem(L, e, equal);
    if (i) // 找到
    {
        if (i == 1) // 是首元结点
        {
            *p = NULL;
            return L;
        }
        *p = L;
        for (j = 2; j < i; j++)
            *p = (*p)->next;
        return (*p)->next;
    }
    return NULL; // 没找到
}

Status DeleteElem(LinkList* L, struct ElemType* e, Status(*equal)(struct ElemType, struct ElemType))
{ // 删除表L中满足条件的结点，并返回TRUE；如无此结点，则返回FALSE。
    // 函数equal()的两形参的关键字相等，返回OK；否则返回ERROR
    LinkList p, q;
    q = Point(*L, *e, equal, &p);
    if (q) // 找到此结点，且q指向该结点
    {
        if (p)                    // 该结点不是首元结点，p指向其前驱
            ListDelete(&p, 2, e); // 将p作为头指针,删除第2个结点
        else                      // 该结点是首元结点
            ListDelete(L, 1, e);
        return TRUE;
    }
    return FALSE;
}

void ListTraverse(LinkList L, void (*vi)(struct ElemType))
{ // 初始条件：线性表L已存在。操作结果：依次对L的每个数据元素调用函数vi()
    LinkList p = L;
    while (p)
    {
        vi(p->data);
        p = p->next;
    }
    printf("\n");
}
#endif

// 图的邻接表存储的基本操作

// 初始条件：图 G 存在，u 和 G 中顶点有相同特征
// 操作结果：若 G 中存在顶点 u，则返回该顶点在图中位置；否则返回 -1
int LocateVex(struct ALGraph G, VertexType u)
{
    int i;
    for (i = 0; i < G.vexnum; ++i)
        if (strcmp(u, G.vertices[i].data) == 0)
            return i;
    return -1;
}

// 采用邻接表存储结构，构造没有相关信息图或网G(用一个函数构造4种图)

void CreateGraph(struct ALGraph* G)
{                      // 采用邻接表存储结构，构造没有相关信息图或网G(用一个函数构造4种图)
    int i, j, k, w;    // w是权值
    VertexType va, vb; // 连接边或弧的2顶点
    struct ElemType e;
    printf("请输入图的类型(有向图:0,有向网:1,无向图:2,无向网:3): ");
    scanf("%d", (int*)&G->kind);
    printf("请输入图的顶点数,边数: ");
    scanf("%d,%d", &G->vexnum, &G->arcnum);
    printf("请输入%d个顶点的值(<%d个字符):\n", G->vexnum, MAX_NAME);
    for (i = 0; i < G->vexnum; ++i) // 构造顶点向量
    {
        scanf("%s", G->vertices[i].data);
        G->vertices[i].firstarc = NULL; // 初始化与该顶点有关的出弧链表
    }
    if (G->kind % 2) // 网
        printf("请输入每条弧(边)的权值、弧尾和弧头(以空格作为间隔):\n");
    else // 图
        printf("请输入每条弧(边)的弧尾和弧头(以空格作为间隔):\n");
    for (k = 0; k < G->arcnum; ++k) // 构造相关弧链表
    {
        if (G->kind % 2) // 网
            scanf("%d%s%s", &w, va, vb);
        else // 图
            scanf("%s%s", va, vb);
        i = LocateVex(*G, va); // 弧尾
        j = LocateVex(*G, vb); // 弧头
        e.info = NULL;         // 给待插表结点e赋值，图无权
        e.adjvex = j;          // 弧头
        if (G->kind % 2)       // 网
        {
            e.info = (int*)malloc(sizeof(int)); // 动态生成存放权值的空间
            *(e.info) = w;
        }
        ListInsert(&G->vertices[i].firstarc, 1, e); // 插在第i个元素(出弧)的表头，在bo2-8.cpp中
        if (G->kind >= 2)                           // 无向图或网，产生第2个表结点，并插在第j个元素(入弧)的表头
        {
            e.adjvex = i;                               // e.info不变，不必再赋值
            ListInsert(&G->vertices[j].firstarc, 1, e); // 插在第j个元素的表头，在bo2-8.cpp中
        }
    }
}

// 采用邻接表存储结构，由文件构造没有相关信息图或网G(用一个函数构造4种图)
void CreateGraphF(struct ALGraph* G)
{
    int i, j, k, w;    // w是权值
    VertexType va, vb; // 连接边或弧的2顶点
    struct ElemType e;
    char filename[13];
    FILE* graphlist;
    printf("请输入数据文件名(f7-1.txt或f7-2.txt)：");
    scanf("%s", filename);
    printf("请输入图的类型(有向图:0,有向网:1,无向图:2,无向网:3): ");
    scanf("%d", (int*)&G->kind);
    graphlist = fopen(filename, "r"); // 以读的方式打开数据文件，并以graphlist表示
    fscanf(graphlist, "%d", &G->vexnum);
    fscanf(graphlist, "%d", &G->arcnum);
    for (i = 0; i < G->vexnum; ++i) // 构造顶点向量
    {
        fscanf(graphlist, "%s", G->vertices[i].data);
        G->vertices[i].firstarc = NULL; // 初始化与该顶点有关的出弧链表
    }
    for (k = 0; k < G->arcnum; ++k) // 构造相关弧链表
    {
        if (G->kind % 2) // 网
            fscanf(graphlist, "%d%s%s", &w, va, vb);
        else // 图
            fscanf(graphlist, "%s%s", va, vb);
        i = LocateVex(*G, va); // 弧尾
        j = LocateVex(*G, vb); // 弧头
        e.info = NULL;         // 给待插表结点e赋值，图无权
        e.adjvex = j;          // 弧头
        if (G->kind % 2)       // 网
        {
            e.info = (int*)malloc(sizeof(int)); // 动态生成存放权值的空间
            *(e.info) = w;
        }
        ListInsert(&G->vertices[i].firstarc, 1, e); // 插在第i个元素(出弧)的表头，在bo2-8.cpp中
        if (G->kind >= 2)                           // 无向图或网，产生第2个表结点，并插在第j个元素(入弧)的表头
        {
            e.adjvex = i;                               // e.info不变，不必再赋值
            ListInsert(&G->vertices[j].firstarc, 1, e); // 插在第j个元素的表头，在bo2-8.cpp中
        }
    }
    fclose(graphlist); // 关闭数据文件
}

void DestroyGraph(struct ALGraph* G)
{ // 初始条件：图G存在。操作结果：销毁图G
    int i;
    struct ElemType e;
    for (i = 0; i < G->vexnum; ++i)         // 对于所有顶点
        if (G->kind % 2)                    // 网
            while (G->vertices[i].firstarc) // 对应的弧或边链表不空
            {
                ListDelete(&G->vertices[i].firstarc, 1, &e); // 删除链表的第1个结点，并将值赋给e
                if (e.adjvex > i)                            // 顶点序号>i(保证动态生成的权值空间只释放1次)
                    free(e.info);
            }
        else                                       // 图
            DestroyList(&G->vertices[i].firstarc); // 销毁弧或边链表，在bo2-8.cpp中
    G->vexnum = 0;                                 // 顶点数为0
    G->arcnum = 0;                                 // 边或弧数为0
}

// 初始条件：图G存在，v是G中某个顶点的序号。操作结果：返回v的值
VertexType* GetVex(struct ALGraph G, int v)
{
    if (v >= G.vexnum || v < 0)
        exit(ERROR);
    return G.vertices[v].data;
}

// 初始条件：图G存在，v是G中某个顶点。操作结果：对v赋新值value
Status PutVex(struct ALGraph* G, VertexType v, VertexType value)
{
    int i;
    i = LocateVex(*G, v);
    if (i > -1) // v是G的顶点
    {
        strcpy(G->vertices[i].data, value);
        return OK;
    }
    return ERROR;
}

// 初始条件：图G存在，v是G中某个顶点
// 操作结果：返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回-1
int FirstAdjVex(struct ALGraph G, VertexType v)
{
    struct ArcNode* p;
    int v1;
    v1 = LocateVex(G, v); // v1为顶点v在图G中的序号
    p = G.vertices[v1].firstarc;
    if (p)
        return p->data.adjvex;
    else
        return -1;
}

// DeleteArc()、DeleteVex()和NextAdjVex()要调用的函数
Status equalvex(struct ElemType a, struct ElemType b)
{
    if (a.adjvex == b.adjvex)
        return OK;
    else
        return ERROR;
}

// 初始条件：图G存在，v是G中某个顶点，w是v的邻接顶点
// 操作结果：返回v的(相对于w的)下一个邻接顶点的序号。若w是v的最后一个邻接点，则返回-1
int NextAdjVex(struct ALGraph G, VertexType v, VertexType w)
{
    LinkList p, p1; // p1在Point()中用作辅助指针，Point()在func2-1.cpp中
    struct ElemType e;
    int v1;
    v1 = LocateVex(G, v);                                 // v1为顶点v在图G中的序号
    e.adjvex = LocateVex(G, w);                           // e.adjvex为顶点w在图G中的序号
    p = Point(G.vertices[v1].firstarc, e, equalvex, &p1); // p指向顶点v的链表中邻接顶点为w的结点
    if (!p || !p->next)                                   // 没找到w或w是最后一个邻接点
        return -1;
    else                             // p->data.adjvex==w
        return p->next->data.adjvex; // 返回v的(相对于w的)下一个邻接顶点的序号
}

// 初始条件：图G存在，v和图中顶点有相同特征
// 操作结果：在图G中增添新顶点v(不增添与顶点相关的弧，留待InsertArc()去做)
void InsertVex(struct ALGraph* G, VertexType v)
{
    strcpy(G->vertices[G->vexnum].data, v); // 构造新顶点向量
    G->vertices[G->vexnum].firstarc = NULL;
    G->vexnum++; // 图G的顶点数加1
}

// 初始条件：图G存在，v是G中某个顶点。操作结果：删除G中顶点v及其相关的弧
Status DeleteVex(struct ALGraph* G, VertexType v)
{
    int i, j, k;
    struct ElemType e;
    LinkList p, p1;
    j = LocateVex(*G, v); // j是顶点v的序号
    if (j < 0)            // v不是图G的顶点
        return ERROR;
    i = ListLength(G->vertices[j].firstarc); // 以v为出度的弧或边数，在bo2-8.cpp中
    G->arcnum -= i;                          // 边或弧数-i
    if (G->kind % 2)                         // 网
        while (G->vertices[j].firstarc)      // 对应的弧或边链表不空
        {
            ListDelete(&G->vertices[j].firstarc, 1, &e); // 删除链表的第1个结点，并将值赋给e
            free(e.info);                                // 释放动态生成的权值空间
        }
    else                                       // 图
        DestroyList(&G->vertices[j].firstarc); // 销毁弧或边链表，在bo2-8.cpp中
    G->vexnum--;                               // 顶点数减1
    for (i = j; i < G->vexnum; i++)            // 顶点v后面的顶点前移
        G->vertices[i] = G->vertices[i + 1];
    for (i = 0; i < G->vexnum; i++) // 删除以v为入度的弧或边且必要时修改表结点的顶点位置值
    {
        e.adjvex = j;
        p = Point(G->vertices[i].firstarc, e, equalvex, &p1); // Point()在func2-1.cpp中
        if (p)                                                // 顶点i的邻接表上有v为入度的结点
        {
            if (p1)                                // p1指向p所指结点的前驱
                p1->next = p->next;                // 从链表中删除p所指结点
            else                                   // p指向首元结点
                G->vertices[i].firstarc = p->next; // 头指针指向下一结点
            if (G->kind < 2)                       // 有向
            {
                G->arcnum--;            // 边或弧数-1
                if (G->kind == 1)       // 有向网
                    free(p->data.info); // 释放动态生成的权值空间
            }
            free(p); // 释放v为入度的结点
        }
        for (k = j + 1; k <= G->vexnum; k++) // 对于adjvex域>j的结点，其序号-1
        {
            e.adjvex = k;
            p = Point(G->vertices[i].firstarc, e, equalvex, &p1); // Point()在func2-1.cpp中
            if (p)
                p->data.adjvex--; // 序号-1(因为前移)
        }
    }
    return OK;
}

// 初始条件：图G存在，v和w是G中两个顶点
// 操作结果：在G中增添弧<v,w>，若G是无向的，则还增添对称弧<w,v>
Status InsertArc(struct ALGraph* G, VertexType v, VertexType w)
{
    struct ElemType e;
    int i, j;
    i = LocateVex(*G, v); // 弧尾或边的序号
    j = LocateVex(*G, w); // 弧头或边的序号
    if (i < 0 || j < 0)
        return ERROR;
    G->arcnum++; // 图G的弧或边的数目加1
    e.adjvex = j;
    e.info = NULL;   // 初值
    if (G->kind % 2) // 网
    {
        e.info = (int*)malloc(sizeof(int)); // 动态生成存放权值的空间
        printf("请输入弧(边)%s→%s的权值: ", v, w);
        scanf("%d", e.info);
    }
    ListInsert(&G->vertices[i].firstarc, 1, e); // 将e插在弧尾的表头，在bo2-8.cpp中
    if (G->kind >= 2)                           // 无向，生成另一个表结点
    {
        e.adjvex = i;                               // e.info不变
        ListInsert(&G->vertices[j].firstarc, 1, e); // 将e插在弧头的表头
    }
    return OK;
}

// 初始条件：图G存在，v和w是G中两个顶点
// 操作结果：在G中删除弧<v,w>，若G是无向的，则还删除对称弧<w,v>
Status DeleteArc(struct ALGraph* G, VertexType v, VertexType w)
{
    int i, j;
    Status k;
    struct ElemType e;
    i = LocateVex(*G, v); // i是顶点v(弧尾)的序号
    j = LocateVex(*G, w); // j是顶点w(弧头)的序号
    if (i < 0 || j < 0 || i == j)
        return ERROR;
    e.adjvex = j;
    k = DeleteElem(&G->vertices[i].firstarc, &e, equalvex); // 在func2-1.cpp中
    if (k)                                                  // 删除成功
    {
        G->arcnum--;     // 弧或边数减1
        if (G->kind % 2) // 网
            free(e.info);
        if (G->kind >= 2) // 无向，删除对称弧<w,v>
        {
            e.adjvex = i;
            DeleteElem(&G->vertices[j].firstarc, &e, equalvex);
        }
        return OK;
    }
    else // 没找到待删除的弧
    {
        return ERROR;
    }
}

Boolean visited[MAX_VERTEX_NUM]; // 访问标志数组(全局量)
void (*VisitFunc)(char* v);      // 函数变量(全局量)
// 从第v个顶点出发递归地深度优先遍历图G。算法7.5
void DFS(struct ALGraph G, int v)
{
    int w;
    visited[v] = TRUE;             // 设置访问标志为TRUE(已访问)
    VisitFunc(G.vertices[v].data); // 访问第v个顶点
    for (w = FirstAdjVex(G, G.vertices[v].data); w >= 0; w = NextAdjVex(G, G.vertices[v].data, G.vertices[w].data))
        if (!visited[w])
            DFS(G, w); // 对v的尚未访问的邻接点w递归调用DFS
}

// 对图G作深度优先遍历。算法7.4
void DFSTraverse(struct ALGraph G, void (*Visit)(char*))
{
    int v;
    VisitFunc = Visit; // 使用全局变量VisitFunc，使DFS不必设函数指针参数
    for (v = 0; v < G.vexnum; v++)
        visited[v] = FALSE; // 访问标志数组初始化
    for (v = 0; v < G.vexnum; v++)
        if (!visited[v])
            DFS(G, v); // 对尚未访问的顶点调用DFS
    printf("\n");
}

typedef int QElemType; // 队列元素类型

#if 1
typedef struct QNode
{
    QElemType data;     /* 数据域 */
    struct QNode* next; /* 指针域 */
} QNode, * QueuePtr;
typedef struct LinkQueue
{
    QueuePtr front; // 队头指针
    QueuePtr rear;  // 队尾指针
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
{                                                         // 构造一个空队列 Q
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode)); /* 单链表的头结点 */

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
    if (!(p = (QueuePtr)malloc(sizeof(QNode)))) // 存储分配失败
        exit(-1);
    p->data = e;
    p->next = NULL;    /* 新结点的 next 为空 */
    Q->rear->next = p; /* 上一次的尾指针指向新的结点 */
    Q->rear = p;       /* 新的尾指针 */
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

//按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。算法7.6
void BFSTraverse(struct ALGraph G, void (*Visit)(char*))
{
    int v, u, w;
    LinkQueue Q;
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = FALSE;        // 置初值
    InitQueue(&Q);                 // 置空的辅助队列Q
    for (v = 0; v < G.vexnum; v++) // 如果是连通图，只v=0就遍历全图
        if (!visited[v])           // v尚未访问
        {
            visited[v] = TRUE;
            Visit(G.vertices[v].data);
            EnQueue(&Q, v);        // v入队列
            while (!QueueEmpty(Q)) // 队列不空
            {
                DeQueue(&Q, &u); // 队头元素出队并置为u
                for (w = FirstAdjVex(G, G.vertices[u].data); w >= 0; w = NextAdjVex(G, G.vertices[u].data, G.vertices[w].data))
                    if (!visited[w]) // w为u的尚未访问的邻接顶点
                    {
                        visited[w] = TRUE;
                        Visit(G.vertices[w].data);
                        EnQueue(&Q, w); // w入队
                    }
            }
        }
    printf("\n");
}

// 从第v个顶点出发递归地深度优先遍历图G。仅适用于邻接表存储结构
void DFS1(struct ALGraph G, int v, void (*Visit)(char*))
{
    struct ArcNode* p;                               // p指向表结点
    visited[v] = TRUE;                               // 设置访问标志为TRUE(已访问)
    Visit(G.vertices[v].data);                       // 访问该顶点
    for (p = G.vertices[v].firstarc; p; p = p->next) // p依次指向v的邻接顶点
        if (!visited[p->data.adjvex])
            DFS1(G, p->data.adjvex, Visit); // 对v的尚未访问的邻接点递归调用DFS1
}

// 对图G作深度优先遍历。DFS1设函数指针参数
void DFSTraverse1(struct ALGraph G, void (*Visit)(char*))
{
    int v;
    for (v = 0; v < G.vexnum; v++)
        visited[v] = FALSE;        // 访问标志数组初始化，置初值为未被访问
    for (v = 0; v < G.vexnum; v++) // 如果是连通图，只v=0就遍历全图
        if (!visited[v])           // v尚未被访问
            DFS1(G, v, Visit);     // 对v调用DFS1
    printf("\n");
}

// 按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。仅适用于邻接表结构
void BFSTraverse1(struct ALGraph G, void (*Visit)(char*))
{
    int v, u;
    struct ArcNode* p; // p指向表结点
    LinkQueue Q;       // 链队列类型
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = FALSE;        // 置初值为未被访问
    InitQueue(&Q);                 // 初始化辅助队列Q
    for (v = 0; v < G.vexnum; v++) // 如果是连通图，只v=0就遍历全图
        if (!visited[v])           // v尚未被访问
        {
            visited[v] = TRUE;         // 设v为已被访问
            Visit(G.vertices[v].data); // 访问v
            EnQueue(&Q, v);            // v入队列
            while (!QueueEmpty(Q))     // 队列不空
            {
                DeQueue(&Q, &u);                                 // 队头元素出队并置为u
                for (p = G.vertices[u].firstarc; p; p = p->next) // p依次指向u的邻接顶点
                    if (!visited[p->data.adjvex])                // u的邻接顶点尚未被访问
                    {
                        visited[p->data.adjvex] = TRUE;         // 该邻接顶点设为已被访问
                        Visit(G.vertices[p->data.adjvex].data); // 访问该邻接顶点
                        EnQueue(&Q, p->data.adjvex);            // 入队该邻接顶点序号
                    }
            }
        }
    printf("\n");
}

// 输出图的邻接矩阵G
void Display(struct ALGraph G)
{
    int i;
    struct ArcNode* p;
    switch (G.kind)
    {
    case DG:
        printf("有向图\n");
        break;
    case DN:
        printf("有向网\n");
        break;
    case UDG:
        printf("无向图\n");
        break;
    case UDN:
        printf("无向网\n");
    }
    printf("%d个顶点：\n", G.vexnum);
    for (i = 0; i < G.vexnum; ++i)
        printf("%s ", G.vertices[i].data);
    printf("\n%d条弧(边):\n", G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        p = G.vertices[i].firstarc;
        while (p)
        {
            if (G.kind <= 1 || i < p->data.adjvex) // 有向或无向两次中的一次
            {
                printf("%s→%s ", G.vertices[i].data, G.vertices[p->data.adjvex].data);
                if (G.kind % 2) // 网
                    printf(":%d ", *(p->data.info));
            }
            p = p->nextarc;
        }
        printf("\n");
    }
}

/************************ end of file **************************/

