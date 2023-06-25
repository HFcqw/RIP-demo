#include"try.h"
void initNetwork(Network* network);
void initNetworkFile(Network* network);
void initRouter(Network* network);
void initDesNetwork(Network* network);
void initNeighborCount(Network* network);
int getRouterPos(Network* g, RouterType vertex);
int getFirstNeighbor(Network* g, RouterType vertex);
int getNextNeighbor(Network* g, RouterType vertex1, RouterType vertex2);
int initRoutingTable(Router* router);
void triggerUpdate(Network* network, RouterType routerID);
void updateRouter(Network* network, RouterType routerID);
/*********
* 一、初始化。配置路由器的图和直连网络（手动输入或读取文件）
*********/

void initNetwork(Network* network) {    //初始化、销毁操作
    Neighbor* e;
    network->routerCount = network->edgeCount = network->networkCount = 0;
    if (network->RL == NULL) return;
    for (int i = 0; i < MAX_ROUTERS; ++i) {
        network->RL[i].dirnetworkCount = 0;
        network->RL[i].neighborCount = 0;
        network->RL[i].rtcount = 0;
        network->RL[i].routerID = NULL;
        network->RL[i].routingTable = NULL;
        network->RL[i].neighbors = NULL;
    }
}

void initNetworkFile(Network* network) {   // 采用邻接表存储结构，由文本文件构造整个网络
    FILE* fp;   //一、创建直连路由器构成的图
    FILE* ff;   //二、为该图添加直连网络
    errno_t err1 = fopen_s(&fp, "output/config/routers.txt", "r"); // 以只读的方式打开文本文件，并以fp表示
    errno_t err2 = fopen_s(&ff, "output/config/networks.txt", "r"); // 以只读的方式打开文本文件，并以ff表示
    if (fp == NULL) {
        perror("\t\tfp初始化失败：error opening file!");
        exit(0);
    }
    if (ff == NULL) {
        perror("\t\tff初始化失败：error opening file!");
        exit(0);
    }
    int i, j, k;
    Neighbor* e;
    fscanf_s(fp, "%d %d ", &network->routerCount, &network->edgeCount);
    for (i = 0; i < network->routerCount; i++) {
        char ch = fgetc(fp);
        network->RL[i].routerID = ch;
        network->RL[i].neighbors = NULL;    //初始化表头指针域为NULL
        //printf("%c", network->RL[i].routerID);
    }
    for (k = 0; k < network->edgeCount; k++) {  // 添加邻居
        fscanf_s(fp, "%d %d", &i, &j);
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成一个新节点
        e->neighborID = j;
        e->distance = 1;
        e->next = network->RL[i].neighbors; //头插法建立新节点
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成另一个对称的新的边节点
        e->neighborID = i;
        e->distance = 1;
        e->next = network->RL[j].neighbors;
        network->RL[j].neighbors = e;
    }

    fscanf_s(ff, "%d", &network->networkCount);
    //printf("%d\n", network->networkCount);
    for (i = 0; i < network->routerCount; i++) {
        fscanf_s(ff, "%d", &network->RL[i].dirnetworkCount);
        //printf("%d-", network->RL[i].dirnetworkCount);
        for (j = 0; j < network->routerCount;j++) {
            char c = fgetc(ff);
            //fscanf_s(ff, "%c ", &network->RL[i].dirNetworkID[j]);  
            if (j == 0)  continue;
            else{ 
                //printf("%c", c);
                network->RL[i].dirNetworkID[j-1] = c;
                //printf("%c", network->RL[i].dirNetworkID[j-1]);
            }
        }
    }
    fclose(fp); // 关闭数据文件fp
    fclose(ff); // 关闭数据文件ff
}

void initRouter(Network* network) {    // 手动输入网络路由器配置
    int i, j, k;
    Neighbor* e;
    printf("\t\t请输入网络中路由器数:\n\t\t");
    scanf_s("%d", &network->routerCount);   //给出图的顶点数n
    getchar();
    printf("\t\t请输入路由器构成的图的边数:\n\t\t");
    scanf_s("%d", &network->edgeCount);   //给出图的边数e
    getchar();
    printf("\t\t请输入路由器网络号（输入格式：A B C D E F）:\n\t\t");
    for (i = 0; i < network->routerCount; i++) {    
        scanf_s("%c", &network->RL[i].routerID,1);    //输入n个字符，表示n个顶点的数据元素的值（结点集）
        network->RL[i].neighbors = NULL;    //初始化表头指针域为NULL
    }
    for (k = 0; k < network->edgeCount; k++) {  
        printf("\t\t请输入边（Vi,Vj）上的顶点序号对i j（输入整数，以空格间隔，格式：0 1）：\n\t\t");
        scanf_s("%d %d", &i, &j);   //输入e行数据对，表示每条边的两个顶点编号,构建邻接表（边集）
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成一个新节点
        e->neighborID = j;
        e->next = network->RL[i].neighbors; //头插法建立新节点
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成另一个对称的新的边节点
        e->neighborID = i;
        e->next = network->RL[j].neighbors;
        network->RL[j].neighbors = e;
    }
}
void initDesNetwork(Network* network) {    // 手动配置路由器的直连网络
    int i, j, k;
    printf("\t\t请输入网络总数（输入整型数字）:\n\t\t");
    scanf_s("%d", &network->networkCount);
    for (i = 0; i < network->routerCount; i++) {
        printf("\t\t请输入路由器 %c 的直连网络数（输入整数）:\n\t\t", network->RL[i].routerID);
        scanf_s("%d", &network->RL[i].dirnetworkCount);
        //printf("%d", network->RL[i].dirnetworkCount);
        for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
            printf("\t\t请输入路由器 %c 直连网络的网络号（输入单个字符）: ", network->RL[i].routerID);
            scanf_s("%c", &network->RL[i].dirNetworkID[j]);
            char c = getchar();
            network->RL[i].dirNetworkID[j] = c;
            //printf("%c", network->RL[i].dirNetworkID[j]);
        }
    }
}

void initNeighborCount(Network* network) {  //获取邻居路由器数量，初始化NeighborCount
    int i, j, k;
    for (i = 0; i < network->routerCount; i++) {
        k = 0;
        Neighbor* p = network->RL[i].neighbors;
        while (p != NULL) {
            p = p->next;
            k++;
        }
        network->RL[i].neighborCount = k;
    }
}

/*********
* 二、图的基本操作。
* * 添加路由器、删除路由器、添加边、删除边、添加直连网络、删除直连网络、销毁图
* * 查找图中是否存在某个顶点，获取某个顶点的第一个邻接结点、获取某个顶点的邻接结点的下一个结点
*********/

void insertRouter(Network* network) {
    if (network->routerCount == MAX_ROUTERS) return;
    int i, j, k;
    Neighbor* e;
    printf("\t\t请输入该路由器的网络号:");
    network->routerCount++;
    scanf_s("%c", &network->RL[network->routerCount-1].routerID, 1);
    getchar();
    /*
    if (getRouterPos(network, network->RL[network->routerCount-1].routerID) != -1) {
        printf("\t\t该路由器已存在");
        return;
    }
    */
    printf("\t\t请输入该路由器的邻居路由器数:\n\t\t");
    scanf_s("%d", &network->RL[network->routerCount-1].neighborCount);
    getchar();
    //printf("\t\t请输入该路由器的邻居路由器数:\n\t\t");
    network->edgeCount += network->RL[network->routerCount-1].neighborCount;
    network->RL[network->routerCount].neighbors = NULL;    
    for (k = 0; k < network->RL[network->routerCount-1].neighborCount; k++) {
        printf("\t\t请输入边（Vi,Vj）上的顶点序号对i j（输入整型数字，以空格间隔，格式：0 1）：\n\t\t");
        scanf_s("%d %d", &i, &j);   //输入e行数据对，表示每条边的两个顶点编号,构建邻接表（边集）
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成一个新节点
        e->neighborID = j;
        e->next = network->RL[i].neighbors; //头插法建立新节点
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成另一个对称的新的边节点
        e->neighborID = i;
        e->next = network->RL[j].neighbors;
        network->RL[j].neighbors = e;
    }
    i = network->routerCount;
    printf("\t\t请输入路由器 %c 的直连网络数（输入整型数字）:\n\t\t", network->RL[i].routerID);
    scanf_s("%d", &network->RL[i].dirnetworkCount);
    network->networkCount += network->RL[i].dirnetworkCount;
    for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
        printf("\t\t请输入路由器 %c 直连网络的网络号（输入单个字符）: ", network->RL[i].routerID);
        scanf_s("%c", &network->RL[i].dirNetworkID[j]);
        char c = getchar();
        network->RL[i].dirNetworkID[j] = c;
     }
}

void deleteRouter(Network* g) {
    RouterType v;
    printf("\t\t请输入该路由器的网络号:");
    scanf_s("%c", &v, 1);
    //getchar();
    int n = getRouterPos(g, v);
    if (n == -1) return;
    //1.遍历删除顶点的边表，找到所关联的顶点有哪些
    Neighbor* p = g->RL[v].neighbors;
    int k;
    Neighbor* t = NULL;
    Neighbor* s;
    while (p != NULL){
        k = p->neighborID;
        s = g->RL[k].neighbors; //s指向与删除顶点关联的顶点的第一个边结点
        while (s != NULL && s->neighborID != v) {   //遍历边表有没有要删除的顶点的序号
            t = s;  //t指向s，保存前驱
            s = s->next;
        }
        if (s != NULL){ //则此时已经找到了要删除顶点的序号
            if (t == NULL) {  //删除的节点正好是第一个，因为t一开始就初始化为NULL，没有经过上面的while循环
                g->RL[k].neighbors = s->next;
            }else{
                t->next = s->next;  //t指向删除节点序号的前驱，s指向要删除节点的序号
            }
            free(s);
        }
        //其他顶点的边表中没有要删除的顶点的序号了，可以放心删除要删除的顶点所关联的边表节点，因为其他顶点已经解除了和这个顶点的关系
        g->RL[v].neighbors = p->next;//头删
        free(p);
        p = g->RL[v].neighbors;//第一个边结点结束，开始对下一个边表结点查找
    }
    g->routerCount--;
    g->RL[v].routerID = g->RL[g->routerCount].routerID;//用最后一个结点的覆盖删除的
    g->RL[v].neighbors = g->RL[g->routerCount].neighbors;

    //调整信息，把原来与最后一个顶点相连的顶点的边信息的dest修改为v（即删除的顶点的位置），其他顶点的边表中关于最后一个顶点的序号改为v
    s = g->RL[v].neighbors;//s指向最后一个顶点的边表
    while (s != NULL){
        k = s->neighborID;//因为是无向图，我的边表里有你，你的边表里也有我，在最后一个顶点的边表中找到关联的其他顶点序号
        p = g->RL[k].neighbors;//p指向与最后一个顶点关联的顶点的边表，把其中与最后第一顶点有关的序号改为v
        while (p != NULL){
            if (p->neighborID == g->routerCount){
                p->neighborID = v;
                break;
            }
            p = p->next;
        }
        s = s->next;
    }
}

void insertEdge(Network* g){ //插入边
    RouterType vertex1, vertex2;
    printf("\t\t请输入边（Vi,Vj）上的顶点序号对i j（输入整型数字，以空格间隔，格式：0 1）：\n\t\t");
    scanf_s("%d %d", &vertex1, &vertex2);
    int v1 = getRouterPos(g, vertex1);
    int v2 = getRouterPos(g, vertex2);
    if (v1 == -1 || v2 == -1) return;
    Neighbor* s;
    //V1 --> V2  创建一个边表类型的节点，一共是8字节
    s = (Neighbor*)malloc(sizeof(Neighbor));
    assert(s != NULL);
    s->neighborID = v2;
    s->next = g->RL[v1].neighbors;//这里采用头插法，越后面调用方法的边会在链表前面显示
    g->RL[v1].neighbors = s;

    //V2 --> V1
    s = (Neighbor*)malloc(sizeof(Neighbor));
    assert(s != NULL);
    s->neighborID = v1;
    s->next = g->RL[v2].neighbors;
    g->RL[v2].neighbors = s;
    g->edgeCount++;
}

void deleteEdge(Network *g){ //删除边
    RouterType vertex1, vertex2;
    printf("\t\t请输入边（Vi,Vj）上的顶点序号对i j（输入整型数字，以空格间隔，格式：0 1）：\n\t\t");
    scanf_s("%d %d", &vertex1, &vertex2);
    int v1 = getRouterPos(g, vertex1);
    int v2 = getRouterPos(g, vertex2);
    if (v1 == -1 || v2 == -1) return;
    Neighbor* q = NULL; //q指向p的前驱
    Neighbor* p;
    //v1 -- > v2   如B-C的这一条边
    p = g->RL[v1].neighbors;//p先指向第一个顶点的第一个边结点的指针
    while(p != NULL && p->neighborID != v2){
        q = p;
        p = p->next;
    }
    if(p == NULL) return; //如果p为空，直接返回，无法删除
    if(q == NULL){  //如果p不为空，说明找到的是链表的第一个边结点，q初始化就为NULL
        g->RL[v1].neighbors = p->next;
    }else{
        q->next = p->next;
    }
    free(p);
    //v2 --> v1  如C-B的这一条边，只是换个符号而已
    q = NULL;
    p = g->RL[v2].neighbors;
    while(p->neighborID != v1){   //因为是无向图，B-C有边，则C-B必有边
        q = p;
        p = p->next;
    }
    if(q==NULL){
        g->RL[v2].neighbors = p->next;
    }else{
        q->next = p->next;
    }
    free(p);
    g->edgeCount--;
}

void insertDirNetwork(Network* network) {
    int i, j, k;
    printf("\t\t请输入网络总数（输入整型数字）:\n\t\t");
    scanf_s("%d", &network->networkCount);
    for (i = 0; i < network->routerCount; i++) {
        printf("\t\t请输入路由器 %c 的直连网络数（输入整型数字）:\n\t\t", network->RL[i].routerID);
        scanf_s("%d", &network->RL[i].dirnetworkCount);
        //printf("%d", network->RL[i].dirnetworkCount);
        for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
            printf("\t\t请输入路由器 %c 直连网络的网络号（输入单个字符）: ", network->RL[i].routerID);
            scanf_s("%c", &network->RL[i].dirNetworkID[j]);
            char c = getchar();
            network->RL[i].dirNetworkID[j] = c;
            //printf("%c", network->RL[i].dirNetworkID[j]);
        }
    }
}
void deleteDirNetwork(Network* network) {
    int i, j, k;
    printf("\t\t请输入网络总数（输入整型数字）:\n\t\t");
    scanf_s("%d", &network->networkCount);
    for (i = 0; i < network->routerCount; i++) {
        printf("\t\t请输入路由器 %c 的直连网络数（输入整型数字）:\n\t\t", network->RL[i].routerID);
        scanf_s("%d", &network->RL[i].dirnetworkCount);
        //printf("%d", network->RL[i].dirnetworkCount);
        for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
            printf("\t\t请输入路由器 %c 直连网络的网络号（输入单个字符）: ", network->RL[i].routerID);
            scanf_s("%c", &network->RL[i].dirNetworkID[j]);
            char c = getchar();
            network->RL[i].dirNetworkID[j] = c;
            //printf("%c", network->RL[i].dirNetworkID[j]);
        }
    }
}

int getRouterPos(Network* g, RouterType vertex) {    // 查找图g中是否存在顶点v，存在则返回该顶点在图中位置；否则返回 -1
    for (int i = 0; i < g->routerCount; ++i) {
        if (g->RL[i].routerID == vertex)
            return i;
    }
    return -1;
}

int getFirstNeighbor(Network* g, RouterType vertex) { //获取某个顶点的第一个邻接结点
    int v = getRouterPos(g, vertex);
    if (v == -1) return -1;
    Neighbor* p = g->RL[v].neighbors;
    if (p != NULL) {
        return p->neighborID;
    }
    return -1;
}
int getNextNeighbor(Network* g, RouterType vertex1, RouterType vertex2) { //求某个顶点（vertex1）的邻接结点（vertex2）的下一个结点
    int v1 = getRouterPos(g, vertex1);
    int v2 = getRouterPos(g, vertex2);
    if (v1 == -1 || v2 == -1) return -1;
    Neighbor* p = g->RL[v1].neighbors;
    while (p != NULL && p->neighborID != v2) {
        p = p->next;
    }
    if (p != NULL && p->next != NULL) {
        return p->next->neighborID;
    }
    return -1;
}

/*********
* 三、路由器初始化路由表
*********/
int initRoutingTable(Router* router) {  //目的网络 距离 下一跳
    int i, j = 0, k;
    RTable *r;
    if (router->dirnetworkCount == 0) return -1;
    router->rtcount = router->dirnetworkCount;  //初始化路由表条目，等于直连网络数
    r = (RTable*)malloc(sizeof(RTable)* router->dirnetworkCount);    //生成一个新节点
    assert(r != NULL);
    printf("\t\t路由器 %c 的路由表\n", router->routerID);
    printf("\t\t目的网络   距离   下一跳路由器\n");
    for (i = 0; i < router->dirnetworkCount; i++) {
        r[i].distance = 1;
        r[i].nextHop = '-';
        r[i].networkID = router->dirNetworkID[i];
        router->routingTable = r;
        printf("\t\t%6c%7d%7c", router->routingTable[i].networkID, router->routingTable[i].distance, router->routingTable[i].nextHop);
        printf("\n");
    }
    printf("\n");
}

/*********
* 四、路由器发送路由更新信息给邻居路由器-处理路由信息更新-发送路由更新信息给邻居路由器
* updateRoutingTable函数中，首先初始化路由表项，然后根据直连邻居的信息更新路由表项。之后，通过迭代更新的方式进行路由选择和路由更新，直到没有更新时停止。
*********/

RouterType inputRouterID(Network network) {
input:
    printf("\t\t请输入需要触发更新的路由器名称：");
    char routerID;
    scanf_s("\n%c", &routerID);
    if (getRouterPos(&network, routerID) == -1) {
        printf("\n\t\t路由器 not found\n\n");
        goto input;
    }
    else {
        printf("\n\t\t/***邻接路由器%c的触发更新：***/\n", routerID);
        return routerID;
    }
}

void triggerUpdate(Network* network, RouterType routerID) {    //路由选择和路由更新
    Router* router = NULL;
    router = &network->RL[getRouterPos(network, routerID)];    // 查找指定路由器ID
    int i, j, k;
    Neighbor* neighbor = router->neighbors;
    RTable* rtable;
    int updateFlag = 0;
    while (neighbor != NULL && (updateFlag == 0)) {  // 遍历邻居路由表集合，接收每个邻居发送的路由信息，更新本路由器的路由表
        int updateFlag = 0;
        i = neighbor->neighborID;
        rtable = (RTable*)malloc(sizeof(RTable) * network->RL[i].rtcount);   //创建临时路由表，存放邻居路由器发送的路由表
        printf("\t\t路由信息：邻接路由器 %c -> 路由器 %c ，具体如下\n", network->RL[i].routerID, router->routerID);
        printf("\t\t目的网络   距离   下一跳路由器\n"); 
        for (j = 0; j < network->RL[i].rtcount; j++) {  // 路由更新目标网络ID、距离和下一跳路由器
            rtable[j].distance = network->RL[i].routingTable[j].distance + 1;
            rtable[j].nextHop = network->RL[i].routerID;
            rtable[j].networkID = network->RL[i].routingTable[j].networkID;
            printf("\t\t%6c%7d%7c\n", rtable[j].networkID, rtable[j].distance, rtable[j].nextHop);
        }
        //printf("\n");

        for (int j = 0; j < network->RL[i].rtcount; j++) {  //外层循环是邻居路由器的路由表的路由条目数，内层循环是本路由器的路由条目数   
            int index = j;
            //int* rtnum = (int*)malloc(sizeof(int) * network->RL[i].rtcount);
            for (int k = 0; k < router->rtcount; k++) { //将路由表中原有的所有条目与每一条新收到的路由条目比对，查找是否有新的网络或新的路径
                if (router->routingTable[k].networkID == network->RL[i].routingTable[j].networkID && router->routingTable[k].nextHop == network->RL[i].routingTable[j].nextHop && network->RL[i].routingTable[j].nextHop != router->routerID) {    //如果网络号相等并且下一跳也相等（且下一跳不是本路由器），则不是新的网络也不是新的路径，路由表中已经存在这个网络及路径的记录；
                    index = -1;
                    if (router->routingTable[k].distance > rtable[j].distance && rtable[j].distance < INFINITY) {    //判断是否需要修改路由表中原有的路由条目（更新距离）
                        router->routingTable[k].nextHop = rtable[j].nextHop;
                        router->routingTable[k].distance = rtable[j].distance;
                        router->routingTable[k].networkID = rtable[j].networkID;
                        printf("//修改的路由为：%6c%7d%7c\n",j ,router->routingTable[k].networkID, router->routingTable[k].distance, router->routingTable[k].nextHop);
                    } 
                    break;  //路由表中已经存在这个网络及路径，退出本层循环，执行下一轮外层循环
                }
            }
            if (index != -1) { //给路由器的路由表添加新的路由条目（路由表中还没有这个网络）,网络号不同、距离更短或距离相等但下一跳不同
                if (router->rtcount < MAX_ENTRIES) {
                    updateFlag = 0; 
                    for (int k = 0; k < router->rtcount; k++) { //外层循环是邻居路由器的路由表的路由条目数，内层循环是本路由器的路由条目数
                        if (router->routingTable[k].networkID == rtable[index].networkID && router->routingTable[k].nextHop == rtable[index].nextHop && rtable[index].nextHop != router->routerID && router->routingTable[k].distance >= rtable[index].distance) {  
                            updateFlag = 1; //路由环路
                            printf("\t\t路由器 %c 更新后的路由表\n", router->routerID);
                            printf("\t\t目的网络   距离   下一跳路由器\n");
                            for (j = 0; j < router->rtcount; j++) {  // 路由更新目标网络ID、距离和下一跳路由器
                                printf("\t\t%6c%7d%7c\n", router->routingTable[j].networkID, router->routingTable[j].distance, router->routingTable[j].nextHop);
                            }
                            printf("\n\t\t/***路由器 %c 的路由已经收敛，不需要再添加路由条目***/\n\n", router->routerID);
                            return;
                        }
                    }
                    int Flag = 1;
                    for (int k = 0; k < router->rtcount; k++) { //外层循环是邻居路由器的路由表的路由条目数，内层循环是本路由器的路由条目数
                        if(router->routingTable[k].networkID == rtable[index].networkID && router->routingTable[k].distance < rtable[index].distance)
                        Flag = 0;
                    }
                    if (Flag == 1) {
                        RTable* rtable2 = (RTable*)malloc(sizeof(RTable) * 1);   //创建临时路由表2，存放一条来自邻居路由器新的路由条目，随后添加到本路由器的路由表中
                        rtable2->distance = rtable[index].distance;
                        rtable2->networkID = rtable[index].networkID;
                        rtable2->nextHop = rtable[index].nextHop;
                        router->routingTable[router->rtcount] = *rtable2;
                        //printf("//添加的路由为：%6c%7d%7c\n", rtable2->networkID, rtable2->distance, rtable2->nextHop);
                        router->rtcount++;
                    }
                    else continue;
                }
                else {
                    printf("\t\t/***路由器 %c 的路由表中的路由条目数已达最大值，不能再添加路由条目***/\n\n", router->routerID);
                    return;
                }
            }
        }
        printf("\t\t路由器 %c 更新后的路由表\n", router->routerID);
        printf("\t\t目的网络   距离   下一跳路由器\n");
        for (j = 0; j < router->rtcount; j++) {  // 路由更新目标网络ID、距离和下一跳路由器
            printf("\t\t%6c%7d%7c\n", router->routingTable[j].networkID, router->routingTable[j].distance, router->routingTable[j].nextHop);
        }
        printf("\n");
        neighbor = neighbor->next;
        //Sleep(3000); //每间隔3000ms就更新一次路由器
    }
    updateRouter(network, routerID);
}

void updateRouter(Network* network, RouterType routerID) {
    Router* router = NULL;
    router = &network->RL[getRouterPos(network, routerID)];    // 查找指定路由器ID
    int i, j, k;
    Neighbor* neighbor = router->neighbors;
    RTable* rtable;
    int updateFlag = 0;
    char NRouterID = network->RL[neighbor->neighborID].routerID;
    while (neighbor != NULL && NRouterID != router->routerID && updateFlag != 1) {  //遍历A的邻接路由器
        NRouterID = network->RL[neighbor->neighborID].routerID;
        printf("\t\t/***邻接路由器%c的触发更新：***/\n\n", NRouterID);
        triggerUpdate(network, NRouterID);
        neighbor = neighbor->next;
    }
    /*
    router = &network->RL[getRouterPos(network, routerID)];    // 查找指定路由器ID
    neighbor = router->neighbors;
    updateFlag = 0;
    NRouterID = network->RL[neighbor->neighborID].routerID;
    printf("\t\t/***邻接路由器%c的触发更新：/***\n\n", NRouterID);
    triggerUpdate(network, NRouterID);
    */
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

/*********
* 五、打印运行结果
*********/

void displayRoutingTable(Network network) { // 显示路由表内容
    //printf("\t\tRouting Table\n");
    //printf("\t\tNetworkID   Distance   Next Hop\n");
    
    for (int i = 0; i < network.routerCount; i++) {
        printf("\t\t路由器 % c 的路由表\n",network.RL[i].routerID);
        printf("\t\t目的网络   距离   下一跳路由器\n");
        for (int j = 0; j < network.RL[i].rtcount;j++) {
            printf("\t\t%6c%7d%7c\n", network.RL[i].routingTable[j].networkID, network.RL[i].routingTable[j].distance, network.RL[i].routingTable[j].nextHop);
        }
        //printf("\n");
    }
}
void displayNetworkSetting(Network network) {   // 显示网络配置
    int i, j;
    Neighbor* e;
    if (network.routerCount == 0) {
        printf("\t\t当前网络已初始化\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        printf("\n\t\t路由器%c 的邻接路由器数为 %d", network.RL[i].routerID, network.RL[i].neighborCount);
        if (network.RL[i].neighborCount > 0) {
            printf("，分别为");
            for (e = network.RL[i].neighbors; e != NULL; e = e->next) {
                printf(" %c", network.RL[e->neighborID].routerID);  
            }
        }    
    }
    printf("\n\t\t当前网络中路由器的邻接表如下\n");
    for (i = 0; i < network.routerCount; i++) {
        printf("\t\t%d: %c", i, network.RL[i].routerID);
        for (e = network.RL[i].neighbors; e != NULL; e = e->next) {
            printf(" -> %d", e->neighborID);  //->
        }
        printf("\n");
    }
    if (network.routerCount ==0) {
        printf("\t\t没有直连网络\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        if (network.RL[i].dirnetworkCount == 0) {
                printf("\t\t路由器%c 没有直连网络\n", network.RL[i].routerID);
        }
        else {
            printf("\n\t\t路由器%c 的直连网络数为 %d，路由器的直连网络为", network.RL[i].routerID, network.RL[i].dirnetworkCount);
            for (j = 0; j < network.RL[i].dirnetworkCount; j++) {
                printf(" %c ", network.RL[i].dirNetworkID[j]);
            }
        }
    } 
    printf("\n");
}
void login_func(Network network) {   //系统菜单
start:
    printf("\t\t-------------------------------------------------\n");
    printf("\t\t|||||       欢迎使用RIP模拟演示系统！       |||||\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    系统主控菜单     **************\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**                0、查看当前配置              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                1、加载文件配置              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                2、手动配置网络              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                3、添加路由器                **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                4、删除路由器                **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                5、添加路由器之间的连接      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                6、删除路由器之间的连接      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                7、添加路由器的直连网络      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                8、删除路由器的直连网络      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                9、删除当前网络配置          **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                10、模拟运行RIP              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                100、退出                    **\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    —— by @ccy ——    **************\n");
    printf("\t\t-------------------------------------------------\n\n");
    printf("\t\t请输入选项对应的数字：");
    int n;
    scanf_s("\n%d", &n);
    switch (n) {
    case 0: system("cls");
        printf("\t\t当前网络连接的配置如下：\n\n");
        displayNetworkSetting(network);
        printf("\t\t您已打印完当前网络配置，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 1:
        initNetworkFile(&network);
        initNeighborCount(&network);
        displayNetworkSetting(network);
        printf("\n\t\t您已完成加载文件配置操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 2:
        initRouter(&network);
        initDesNetwork(&network);
        printf("\n\t\t您已完成手动配置网络操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 3:
        insertRouter(&network);
        printf("\n\t\t您已完成添加路由器操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 4:
        deleteRouter(&network);
        printf("\n\t\t您已完成删除路由器操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 5:
        insertEdge(&network);
        printf("\n\t\t您已完成添加路由器连接操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 6:
        deleteEdge(&network);
        printf("\n\t\t您已完成删除路由器连接操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 7:
        insertDirNetwork(&network);
        printf("\n\t\t您已完成添加路由器的直连网络操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 8:
        deleteDirNetwork(&network);
        printf("\n\t\t您已完成删除路由器的直连网络操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 9:
        initNetwork(&network);
        printf("\n\t\t您已完成删除网络配置操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 10: system("cls");
        printf("\n\t\t模拟RIP网络运行如下：\n");
        printf("\t\t初始化路由表\n");
        for (int i = 0; i < network.routerCount; i++) {
            initRoutingTable(&network.RL[i]);
        }
        triggerUpdate(&network, inputRouterID(network));
        printf("\n\t\t打印运行结果：\n");
        displayRoutingTable(network);

        printf("\n\t\t模拟RIP网络的运行结果输出完毕，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 100: printf("\n\t\t您已成功退出\n");
        break;
    default: printf("\t\twrong");
    }
}
int main() {
    clock_t begin = clock();    //计算进程的运算时间
    Network network;
    initNetwork(&network);
    login_func(network);  //系统菜单
    clock_t end = clock();
    printf("\n\t\t运行时间 %.3lf seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}