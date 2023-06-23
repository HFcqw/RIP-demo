#include"try.h"

/*********
* 一、初始化。创建网络和路由器。（手动输入或读取文件）
*********/

void initNetwork(Network* network) {    //初始化、销毁操作
    Neighbor* e;
    network->routerCount = network->edgeCount = network->networkCount = 0;
    if(network->RL != NULL);
    for (int i = 0; i < MAX_ROUTERS; ++i) {
        network->RL[i].dirnetworkCount = 0;
        network->RL[i].neighborCount = 0;
        network->RL[i].routerID = NULL;
        //network->RL[i].routingTable = NULL;
        network->RL[i].neighbors = NULL;
    }
    if(network->DL != NULL);
    for (int i = 0; i < MAX_NETWORKS; ++i) {
        network->DL[i].dnetworkID = NULL;
        network->DL[i].road = NULL;
        network->DL[i].dirrouterCount = NULL;
    }
}

void initNetworkFile(Network* network) {   // 采用邻接表存储结构，由文本文件构造整个网络
    clock_t begin = clock();    //计算进程的运算时间
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
        //e->distance = 1;
        e->next = network->RL[i].neighbors; //头插法建立新节点
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //生成另一个对称的新的边节点
        e->neighborID = i;
        //e->distance = 1;
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
}
void initDesNetwork(Network* network) {    // 手动配置路由器的直连网络
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

/*********
* 二、图的基本操作。添加路由器、删除路由器、添加边、删除边、添加直连网络、删除直连网络、销毁图。
* 获取某个顶点的第一个邻接结点、获取某个顶点的邻接结点的下一个结点
*********/

int getRouterPos(Network* g, RouterType v) {    // 查找图g中是否存在顶点v，存在则返回该顶点在图中位置；否则返回 -1
    for (int i = 0; i < g->routerCount; ++i) {
        if (g->RL[i].routerID == v)
            return i;
    }
    return -1;
}
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

int getFirstNeighbor(Network* g, RouterType vertex){ //获取某个顶点的第一个邻接结点
    int v = getRouterPos(g, vertex);
    if (v == -1) return -1;
    Neighbor* p = g->RL[v].neighbors;
    if (p != NULL) {
        return p->neighborID;
    }
    return -1;
}
int getNextNeighbor(Network* g, RouterType vertex1, RouterType vertex2){ //求某个顶点（vertex1）的邻接结点（vertex2）的下一个结点
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
int initRoutingTable(Network* network,int count) {
    int i, j=0, k;
    int disance = 1;
    char c = '-';
    for (i = 0; i < network->routerCount; i++) {
        network->RL[i].routingTable[j].distance = disance;
        printf(" %d ", network->RL[i].routingTable[j].distance);
        network->RL[i].routingTable[j].nextHop = c;
        printf(" %c ", network->RL[i].routingTable[j].nextHop);
        for (k = 0; k < network->RL[i].neighborCount; k++) {
            network->RL[i].routingTable[j].networkID = network->RL[i].dirNetworkID[k];
            printf(" %c ", network->RL[i].routingTable[j].networkID);
        }
        printf("\n");
    }
    return count = j;
}

/*********
* 四、路由器发送路由更新信息给邻居路由器-处理路由信息更新-发送路由更新信息给邻居路由器
*********/

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
    Router* router = NULL;
    int i;

    // 查找指定路由器ID的路由器
    for (i = 0; i < network->routerCount; i++) {
        if (network->RL[i].routerID == routerID) {
            router = &network->RL[i];
            break;
        }
    }
    if (router == NULL) {
        printf("Router with ID %d not found\n", routerID);
        return;
    }
    // 遍历邻居列表，向每个邻居发送路由更新信息
    Neighbor* neighbor = router->neighbors;
    while (neighbor != NULL) {
        int neighborID = neighbor->neighborID;
        int networkID, distance; // 路由更新信息的目标网络ID和距离

        // 根据路由表中的信息构造路由更新信息
        // ...

        // 发送路由更新信息给邻居路由器
        // ...

        neighbor = neighbor->next;
    }
}
/*
* 	int i, j;
    for (i = 0; i < *count2; i++) {
        rtu[i].d++;
    }
    printf("距离向量加1后的路由表\n");
    prt(rtu, *count2);
    for (i = 0; i < *count2; i++) {      //根据接收的路边表，一项一项进行比对
        for (j = 0; j < *count; j++) {
            if (rt[j].tn == rtu[i].tn) { //如果原来有该目标网络表项
                if (rt[j].next == rtu[i].next) { //如果下一跳地址一样，则直接用新的覆盖原来的
                    rt[j].d = rtu[i].d;
                }
                else if (rt[j].d > rtu[i].d) {
                    //如果下一跳地址不同，则比较两个表项哪个距离更小
                    rt[j].next = rtu[i].next;
                    rt[j].d = rtu[i].d;
                }
                break;
            }
        }
        if (j >= *count) { //如果发来的路由表中有表项在原来rt表中没有，则直接插入到rt表中
            insertTable(rt, count, rtu[i].tn, rtu[i].d, rtu[i].next);
        }
    }
}
*/

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
/*
void updateRoutingTable(Network* network, Router* router, int senderId, int numNetworks, int networks[], int metrics[],RTable* routingTable) {

    for (int i = 0; i < network->networkCount; i++) {
        int network = networks[i];
        int metric = metrics[i];

        // 查找网络在路由表中的位置
        int index = -1;
        for (int j = 0; j < router->dirnetworkCount; j++) {
            if (router->dirNetworkID[j] == network) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            // 毒性逆转
            if (senderId == router->routingTable[senderId][index].nextHop && metric == INFINITY) {
                router->routingTable[senderId][index].metric = INFINITY;
            } else {
                // 更新路由表条目
                int currentMetric = router->routingTable[senderId][index].metric;
                if (metric + 1 < currentMetric) {
                    router->routingTable[senderId][index].nextHop = senderId;
                    router->routingTable[senderId][index].metric = metric + 1;
                }
            }
        }
    }
}
*/

void displayRoutingTable(Network network) { // 显示路由表内容
    //printf("\t\tRouting Table\n");
    //printf("\t\tNetworkID   Distance   Next Hop\n");
    printf("\t\t目的网络   距离   下一跳    \n");
    for (int i = 0; i < network.routerCount; i++) {
        for (int j = 0; j < network.RL[i].dirnetworkCount;j++) {
            printf("\t\t%6c%6d%6c\n", network.RL[i].routingTable[j].networkID, network.RL[i].routingTable[j].distance, network.RL[i].routingTable[j].nextHop);
        }
    }
}
void displayNetworkSetting(Network network) {   // 显示网络配置
    int i, j;
    Neighbor* e;
    if (network.routerCount == 0) {
        printf("\t\t当前系统网络已初始化\n");
        return;
    }
    printf("\t\t当前系统网络中路由器的邻接表\n");
    for (i = 0; i < network.routerCount; i++) {
        printf("\t\t%d: %c", i, network.RL[i].routerID);
        for (e = network.RL[i].neighbors; e; e = e->next) {
            printf(" -> %d", e->neighborID);  //->
        }
        printf("\n");
    }
    if (network.routerCount ==0) {
        printf("\t\t系统中没有直连网络\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        if (network.RL[i].dirnetworkCount == 0) {
                printf("\t\t路由器%c 没有直连网络\n", network.RL[i].routerID);
        }else {
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
    printf("\n");
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
    printf("\t\t**                7、删除当前网络配置          **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                10、运行系统                 **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                100、退出系统                **\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    —— by @ccy ——    **************\n");
    printf("\t\t-------------------------------------------------\n\n");
    int n;
    int count = 0;
    printf("\t\t请输入选项对应的数字：");
    scanf_s("%d", &n);
    switch (n) {
    case 0: system("cls");
        printf("\t\t系统当前网络连接的配置如下：\n\n");
        displayNetworkSetting(network);
        printf("\t\t您已打印完系统当前网络配置，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 1:
        initNetworkFile(&network);
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
        insertEdge(&network);
        printf("\n\t\t您已完成删除路由器连接操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 7:
        initNetwork(&network);
        printf("\n\t\t您已完成删除网络配置操作，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 10: system("cls");
        printf("\n\t\t系统模拟RIP网络运行如下：\n");
        initRoutingTable(&network, count);
        displayRoutingTable(network);
        /*
        while (1) {
            Sleep(3000); //每间隔3000ms就更新一次路由器 
            receiveRIP(rtu, &count2); //收到来自相邻路由器的路由表 
            printf("接收到的路由信息:\n");
            prt(rtu, count2);
            updateRIP(rt, &count, rtu, &count2);
            printf("更新后的路由信息:\n");
            prt(rt, count);
        }
        */
        printf("\n\t\t系统模拟RIP网络运行输出完毕，按任意键返回初始界面\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 100: printf("\n\t\t您已成功退出系统\n");
        break;
    default: printf("\t\twrong");
    }
}
int main() {
    Network network;
    initNetwork(&network);
    login_func(network);  //系统菜单
    return 0;
}