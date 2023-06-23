#include"try.h"

/*********
* һ����ʼ�������������·���������ֶ�������ȡ�ļ���
*********/

void initNetwork(Network* network) {    //��ʼ�������ٲ���
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

void initNetworkFile(Network* network) {   // �����ڽӱ�洢�ṹ�����ı��ļ�������������
    clock_t begin = clock();    //������̵�����ʱ��
    FILE* fp;   //һ������ֱ��·�������ɵ�ͼ
    FILE* ff;   //����Ϊ��ͼ���ֱ������
    errno_t err1 = fopen_s(&fp, "output/config/routers.txt", "r"); // ��ֻ���ķ�ʽ���ı��ļ�������fp��ʾ
    errno_t err2 = fopen_s(&ff, "output/config/networks.txt", "r"); // ��ֻ���ķ�ʽ���ı��ļ�������ff��ʾ
    if (fp == NULL) {
        perror("\t\tfp��ʼ��ʧ�ܣ�error opening file!");
        exit(0);
    }
    if (ff == NULL) {
        perror("\t\tff��ʼ��ʧ�ܣ�error opening file!");
        exit(0);
    }
    int i, j, k;
    Neighbor* e;
    fscanf_s(fp, "%d %d ", &network->routerCount, &network->edgeCount);
    for (i = 0; i < network->routerCount; i++) {
        char ch = fgetc(fp);
        network->RL[i].routerID = ch;
        network->RL[i].neighbors = NULL;    //��ʼ����ͷָ����ΪNULL
        //printf("%c", network->RL[i].routerID);
    }
    for (k = 0; k < network->edgeCount; k++) {  // ����ھ�
        fscanf_s(fp, "%d %d", &i, &j);
        e = (Neighbor*)malloc(sizeof(Neighbor));    //����һ���½ڵ�
        e->neighborID = j;
        //e->distance = 1;
        e->next = network->RL[i].neighbors; //ͷ�巨�����½ڵ�
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //������һ���ԳƵ��µı߽ڵ�
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
    fclose(fp); // �ر������ļ�fp
    fclose(ff); // �ر������ļ�ff
}

void initRouter(Network* network) {    // �ֶ���������·��������
    int i, j, k;
    Neighbor* e;
    printf("\t\t������������·������:\n\t\t");
    scanf_s("%d", &network->routerCount);   //����ͼ�Ķ�����n
    getchar();
    printf("\t\t������·�������ɵ�ͼ�ı���:\n\t\t");
    scanf_s("%d", &network->edgeCount);   //����ͼ�ı���e
    getchar();
    printf("\t\t������·��������ţ������ʽ��A B C D E F��:\n\t\t");
    for (i = 0; i < network->routerCount; i++) {    
        scanf_s("%c", &network->RL[i].routerID,1);    //����n���ַ�����ʾn�����������Ԫ�ص�ֵ����㼯��
        network->RL[i].neighbors = NULL;    //��ʼ����ͷָ����ΪNULL
    }
    for (k = 0; k < network->edgeCount; k++) {  
        printf("\t\t������ߣ�Vi,Vj���ϵĶ�����Ŷ�i j�������������֣��Կո�������ʽ��0 1����\n\t\t");
        scanf_s("%d %d", &i, &j);   //����e�����ݶԣ���ʾÿ���ߵ�����������,�����ڽӱ��߼���
        e = (Neighbor*)malloc(sizeof(Neighbor));    //����һ���½ڵ�
        e->neighborID = j;
        e->next = network->RL[i].neighbors; //ͷ�巨�����½ڵ�
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //������һ���ԳƵ��µı߽ڵ�
        e->neighborID = i;
        e->next = network->RL[j].neighbors;
        network->RL[j].neighbors = e;
    }
}
void initDesNetwork(Network* network) {    // �ֶ�����·������ֱ������
    int i, j, k;
    printf("\t\t���������������������������֣�:\n\t\t");
    scanf_s("%d", &network->networkCount);
    for (i = 0; i < network->routerCount; i++) {
        printf("\t\t������·���� %c ��ֱ���������������������֣�:\n\t\t", network->RL[i].routerID);
        scanf_s("%d", &network->RL[i].dirnetworkCount);
        //printf("%d", network->RL[i].dirnetworkCount);
        for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
            printf("\t\t������·���� %c ֱ�����������ţ����뵥���ַ���: ", network->RL[i].routerID);
            scanf_s("%c", &network->RL[i].dirNetworkID[j]);
            char c = getchar();
            network->RL[i].dirNetworkID[j] = c;
            //printf("%c", network->RL[i].dirNetworkID[j]);
        }
    }
}

/*********
* ����ͼ�Ļ������������·������ɾ��·��������ӱߡ�ɾ���ߡ����ֱ�����硢ɾ��ֱ�����硢����ͼ��
* ��ȡĳ������ĵ�һ���ڽӽ�㡢��ȡĳ��������ڽӽ�����һ�����
*********/

int getRouterPos(Network* g, RouterType v) {    // ����ͼg���Ƿ���ڶ���v�������򷵻ظö�����ͼ��λ�ã����򷵻� -1
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
    printf("\t\t�������·�����������:");
    network->routerCount++;
    scanf_s("%c", &network->RL[network->routerCount-1].routerID, 1);
    getchar();
    /*
    if (getRouterPos(network, network->RL[network->routerCount-1].routerID) != -1) {
        printf("\t\t��·�����Ѵ���");
        return;
    }
    */
    printf("\t\t�������·�������ھ�·������:\n\t\t");
    scanf_s("%d", &network->RL[network->routerCount-1].neighborCount);
    getchar();
    //printf("\t\t�������·�������ھ�·������:\n\t\t");
    network->edgeCount += network->RL[network->routerCount-1].neighborCount;
    network->RL[network->routerCount].neighbors = NULL;    
    for (k = 0; k < network->RL[network->routerCount-1].neighborCount; k++) {
        printf("\t\t������ߣ�Vi,Vj���ϵĶ�����Ŷ�i j�������������֣��Կո�������ʽ��0 1����\n\t\t");
        scanf_s("%d %d", &i, &j);   //����e�����ݶԣ���ʾÿ���ߵ�����������,�����ڽӱ��߼���
        e = (Neighbor*)malloc(sizeof(Neighbor));    //����һ���½ڵ�
        e->neighborID = j;
        e->next = network->RL[i].neighbors; //ͷ�巨�����½ڵ�
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //������һ���ԳƵ��µı߽ڵ�
        e->neighborID = i;
        e->next = network->RL[j].neighbors;
        network->RL[j].neighbors = e;
    }
    i = network->routerCount;
    printf("\t\t������·���� %c ��ֱ���������������������֣�:\n\t\t", network->RL[i].routerID);
    scanf_s("%d", &network->RL[i].dirnetworkCount);
    network->networkCount += network->RL[i].dirnetworkCount;
    for (j = 0; j < network->RL[i].dirnetworkCount; j++) {
        printf("\t\t������·���� %c ֱ�����������ţ����뵥���ַ���: ", network->RL[i].routerID);
        scanf_s("%c", &network->RL[i].dirNetworkID[j]);
        char c = getchar();
        network->RL[i].dirNetworkID[j] = c;
     }
}

void deleteRouter(Network* g) {
    RouterType v;
    printf("\t\t�������·�����������:");
    scanf_s("%c", &v, 1);
    //getchar();
    int n = getRouterPos(g, v);
    if (n == -1) return;
    //1.����ɾ������ı߱��ҵ��������Ķ�������Щ
    Neighbor* p = g->RL[v].neighbors;
    int k;
    Neighbor* t = NULL;
    Neighbor* s;
    while (p != NULL){
        k = p->neighborID;
        s = g->RL[k].neighbors; //sָ����ɾ����������Ķ���ĵ�һ���߽��
        while (s != NULL && s->neighborID != v) {   //�����߱���û��Ҫɾ���Ķ�������
            t = s;  //tָ��s������ǰ��
            s = s->next;
        }
        if (s != NULL){ //���ʱ�Ѿ��ҵ���Ҫɾ����������
            if (t == NULL) {  //ɾ���Ľڵ������ǵ�һ������Ϊtһ��ʼ�ͳ�ʼ��ΪNULL��û�о��������whileѭ��
                g->RL[k].neighbors = s->next;
            }else{
                t->next = s->next;  //tָ��ɾ���ڵ���ŵ�ǰ����sָ��Ҫɾ���ڵ�����
            }
            free(s);
        }
        //��������ı߱���û��Ҫɾ���Ķ��������ˣ����Է���ɾ��Ҫɾ���Ķ����������ı߱�ڵ㣬��Ϊ���������Ѿ�����˺��������Ĺ�ϵ
        g->RL[v].neighbors = p->next;//ͷɾ
        free(p);
        p = g->RL[v].neighbors;//��һ���߽���������ʼ����һ���߱������
    }
    g->routerCount--;
    g->RL[v].routerID = g->RL[g->routerCount].routerID;//�����һ�����ĸ���ɾ����
    g->RL[v].neighbors = g->RL[g->routerCount].neighbors;

    //������Ϣ����ԭ�������һ�����������Ķ���ı���Ϣ��dest�޸�Ϊv����ɾ���Ķ����λ�ã�����������ı߱��й������һ���������Ÿ�Ϊv
    s = g->RL[v].neighbors;//sָ�����һ������ı߱�
    while (s != NULL){
        k = s->neighborID;//��Ϊ������ͼ���ҵı߱������㣬��ı߱���Ҳ���ң������һ������ı߱����ҵ������������������
        p = g->RL[k].neighbors;//pָ�������һ����������Ķ���ı߱�������������һ�����йص���Ÿ�Ϊv
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

void insertEdge(Network* g){ //�����
    RouterType vertex1, vertex2;
    printf("\t\t������ߣ�Vi,Vj���ϵĶ�����Ŷ�i j�������������֣��Կո�������ʽ��0 1����\n\t\t");
    scanf_s("%d %d", &vertex1, &vertex2);
    int v1 = getRouterPos(g, vertex1);
    int v2 = getRouterPos(g, vertex2);
    if (v1 == -1 || v2 == -1) return;
    Neighbor* s;
    //V1 --> V2  ����һ���߱����͵Ľڵ㣬һ����8�ֽ�
    s = (Neighbor*)malloc(sizeof(Neighbor));
    assert(s != NULL);
    s->neighborID = v2;
    s->next = g->RL[v1].neighbors;//�������ͷ�巨��Խ������÷����ı߻�������ǰ����ʾ
    g->RL[v1].neighbors = s;

    //V2 --> V1
    s = (Neighbor*)malloc(sizeof(Neighbor));
    assert(s != NULL);
    s->neighborID = v1;
    s->next = g->RL[v2].neighbors;
    g->RL[v2].neighbors = s;
    g->edgeCount++;
}

void deleteEdge(Network *g){ //ɾ����
    RouterType vertex1, vertex2;
    printf("\t\t������ߣ�Vi,Vj���ϵĶ�����Ŷ�i j�������������֣��Կո�������ʽ��0 1����\n\t\t");
    scanf_s("%d %d", &vertex1, &vertex2);
    int v1 = getRouterPos(g, vertex1);
    int v2 = getRouterPos(g, vertex2);
    if (v1 == -1 || v2 == -1) return;
    Neighbor* q = NULL; //qָ��p��ǰ��
    Neighbor* p;
    //v1 -- > v2   ��B-C����һ����
    p = g->RL[v1].neighbors;//p��ָ���һ������ĵ�һ���߽���ָ��
    while(p != NULL && p->neighborID != v2){
        q = p;
        p = p->next;
    }
    if(p == NULL) return; //���pΪ�գ�ֱ�ӷ��أ��޷�ɾ��
    if(q == NULL){  //���p��Ϊ�գ�˵���ҵ���������ĵ�һ���߽�㣬q��ʼ����ΪNULL
        g->RL[v1].neighbors = p->next;
    }else{
        q->next = p->next;
    }
    free(p);
    //v2 --> v1  ��C-B����һ���ߣ�ֻ�ǻ������Ŷ���
    q = NULL;
    p = g->RL[v2].neighbors;
    while(p->neighborID != v1){   //��Ϊ������ͼ��B-C�бߣ���C-B���б�
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

int getFirstNeighbor(Network* g, RouterType vertex){ //��ȡĳ������ĵ�һ���ڽӽ��
    int v = getRouterPos(g, vertex);
    if (v == -1) return -1;
    Neighbor* p = g->RL[v].neighbors;
    if (p != NULL) {
        return p->neighborID;
    }
    return -1;
}
int getNextNeighbor(Network* g, RouterType vertex1, RouterType vertex2){ //��ĳ�����㣨vertex1�����ڽӽ�㣨vertex2������һ�����
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
* ����·������ʼ��·�ɱ�
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
* �ġ�·��������·�ɸ�����Ϣ���ھ�·����-����·����Ϣ����-����·�ɸ�����Ϣ���ھ�·����
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

    // ����ָ��·����ID��·����
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
/*
* 	int i, j;
    for (i = 0; i < *count2; i++) {
        rtu[i].d++;
    }
    printf("����������1���·�ɱ�\n");
    prt(rtu, *count2);
    for (i = 0; i < *count2; i++) {      //���ݽ��յ�·�߱�һ��һ����бȶ�
        for (j = 0; j < *count; j++) {
            if (rt[j].tn == rtu[i].tn) { //���ԭ���и�Ŀ���������
                if (rt[j].next == rtu[i].next) { //�����һ����ַһ������ֱ�����µĸ���ԭ����
                    rt[j].d = rtu[i].d;
                }
                else if (rt[j].d > rtu[i].d) {
                    //�����һ����ַ��ͬ����Ƚ����������ĸ������С
                    rt[j].next = rtu[i].next;
                    rt[j].d = rtu[i].d;
                }
                break;
            }
        }
        if (j >= *count) { //���������·�ɱ����б�����ԭ��rt����û�У���ֱ�Ӳ��뵽rt����
            insertTable(rt, count, rtu[i].tn, rtu[i].d, rtu[i].next);
        }
    }
}
*/

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
/*
void updateRoutingTable(Network* network, Router* router, int senderId, int numNetworks, int networks[], int metrics[],RTable* routingTable) {

    for (int i = 0; i < network->networkCount; i++) {
        int network = networks[i];
        int metric = metrics[i];

        // ����������·�ɱ��е�λ��
        int index = -1;
        for (int j = 0; j < router->dirnetworkCount; j++) {
            if (router->dirNetworkID[j] == network) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            // ������ת
            if (senderId == router->routingTable[senderId][index].nextHop && metric == INFINITY) {
                router->routingTable[senderId][index].metric = INFINITY;
            } else {
                // ����·�ɱ���Ŀ
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

void displayRoutingTable(Network network) { // ��ʾ·�ɱ�����
    //printf("\t\tRouting Table\n");
    //printf("\t\tNetworkID   Distance   Next Hop\n");
    printf("\t\tĿ������   ����   ��һ��    \n");
    for (int i = 0; i < network.routerCount; i++) {
        for (int j = 0; j < network.RL[i].dirnetworkCount;j++) {
            printf("\t\t%6c%6d%6c\n", network.RL[i].routingTable[j].networkID, network.RL[i].routingTable[j].distance, network.RL[i].routingTable[j].nextHop);
        }
    }
}
void displayNetworkSetting(Network network) {   // ��ʾ��������
    int i, j;
    Neighbor* e;
    if (network.routerCount == 0) {
        printf("\t\t��ǰϵͳ�����ѳ�ʼ��\n");
        return;
    }
    printf("\t\t��ǰϵͳ������·�������ڽӱ�\n");
    for (i = 0; i < network.routerCount; i++) {
        printf("\t\t%d: %c", i, network.RL[i].routerID);
        for (e = network.RL[i].neighbors; e; e = e->next) {
            printf(" -> %d", e->neighborID);  //->
        }
        printf("\n");
    }
    if (network.routerCount ==0) {
        printf("\t\tϵͳ��û��ֱ������\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        if (network.RL[i].dirnetworkCount == 0) {
                printf("\t\t·����%c û��ֱ������\n", network.RL[i].routerID);
        }else {
            printf("\n\t\t·����%c ��ֱ��������Ϊ %d��·������ֱ������Ϊ", network.RL[i].routerID, network.RL[i].dirnetworkCount);
            for (j = 0; j < network.RL[i].dirnetworkCount; j++) {
                printf(" %c ", network.RL[i].dirNetworkID[j]);
            }
        }
    } 
    printf("\n");
}
void login_func(Network network) {   //ϵͳ�˵�
start:
    printf("\n");
    printf("\t\t-------------------------------------------------\n");
    printf("\t\t|||||       ��ӭʹ��RIPģ����ʾϵͳ��       |||||\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    ϵͳ���ز˵�     **************\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**                0���鿴��ǰ����              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                1�������ļ�����              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                2���ֶ���������              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                3�����·����                **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                4��ɾ��·����                **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                5�����·����֮�������      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                6��ɾ��·����֮�������      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                7��ɾ����ǰ��������          **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                10������ϵͳ                 **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                100���˳�ϵͳ                **\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    ���� by @ccy ����    **************\n");
    printf("\t\t-------------------------------------------------\n\n");
    int n;
    int count = 0;
    printf("\t\t������ѡ���Ӧ�����֣�");
    scanf_s("%d", &n);
    switch (n) {
    case 0: system("cls");
        printf("\t\tϵͳ��ǰ�������ӵ��������£�\n\n");
        displayNetworkSetting(network);
        printf("\t\t���Ѵ�ӡ��ϵͳ��ǰ�������ã�����������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 1:
        initNetworkFile(&network);
        displayNetworkSetting(network);
        printf("\n\t\t������ɼ����ļ����ò���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 2:
        initRouter(&network);
        initDesNetwork(&network);
        printf("\n\t\t��������ֶ������������������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 3:
        insertRouter(&network);
        printf("\n\t\t����������·��������������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 4:
        deleteRouter(&network);
        printf("\n\t\t�������ɾ��·��������������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 5:
        insertEdge(&network);
        printf("\n\t\t����������·�������Ӳ���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 6:
        insertEdge(&network);
        printf("\n\t\t�������ɾ��·�������Ӳ���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 7:
        initNetwork(&network);
        printf("\n\t\t�������ɾ���������ò���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 10: system("cls");
        printf("\n\t\tϵͳģ��RIP�����������£�\n");
        initRoutingTable(&network, count);
        displayRoutingTable(network);
        /*
        while (1) {
            Sleep(3000); //ÿ���3000ms�͸���һ��·���� 
            receiveRIP(rtu, &count2); //�յ���������·������·�ɱ� 
            printf("���յ���·����Ϣ:\n");
            prt(rtu, count2);
            updateRIP(rt, &count, rtu, &count2);
            printf("���º��·����Ϣ:\n");
            prt(rt, count);
        }
        */
        printf("\n\t\tϵͳģ��RIP�������������ϣ�����������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 100: printf("\n\t\t���ѳɹ��˳�ϵͳ\n");
        break;
    default: printf("\t\twrong");
    }
}
int main() {
    Network network;
    initNetwork(&network);
    login_func(network);  //ϵͳ�˵�
    return 0;
}