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
* һ����ʼ��������·������ͼ��ֱ�����磨�ֶ�������ȡ�ļ���
*********/

void initNetwork(Network* network) {    //��ʼ�������ٲ���
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

void initNetworkFile(Network* network) {   // �����ڽӱ�洢�ṹ�����ı��ļ�������������
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
        e->distance = 1;
        e->next = network->RL[i].neighbors; //ͷ�巨�����½ڵ�
        network->RL[i].neighbors = e;
        e = (Neighbor*)malloc(sizeof(Neighbor));    //������һ���ԳƵ��µı߽ڵ�
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
        printf("\t\t������ߣ�Vi,Vj���ϵĶ�����Ŷ�i j�������������Կո�������ʽ��0 1����\n\t\t");
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
        printf("\t\t������·���� %c ��ֱ��������������������:\n\t\t", network->RL[i].routerID);
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

void initNeighborCount(Network* network) {  //��ȡ�ھ�·������������ʼ��NeighborCount
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
* ����ͼ�Ļ���������
* * ���·������ɾ��·��������ӱߡ�ɾ���ߡ����ֱ�����硢ɾ��ֱ�����硢����ͼ
* * ����ͼ���Ƿ����ĳ�����㣬��ȡĳ������ĵ�һ���ڽӽ�㡢��ȡĳ��������ڽӽ�����һ�����
*********/

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

void insertDirNetwork(Network* network) {
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
void deleteDirNetwork(Network* network) {
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

int getRouterPos(Network* g, RouterType vertex) {    // ����ͼg���Ƿ���ڶ���v�������򷵻ظö�����ͼ��λ�ã����򷵻� -1
    for (int i = 0; i < g->routerCount; ++i) {
        if (g->RL[i].routerID == vertex)
            return i;
    }
    return -1;
}

int getFirstNeighbor(Network* g, RouterType vertex) { //��ȡĳ������ĵ�һ���ڽӽ��
    int v = getRouterPos(g, vertex);
    if (v == -1) return -1;
    Neighbor* p = g->RL[v].neighbors;
    if (p != NULL) {
        return p->neighborID;
    }
    return -1;
}
int getNextNeighbor(Network* g, RouterType vertex1, RouterType vertex2) { //��ĳ�����㣨vertex1�����ڽӽ�㣨vertex2������һ�����
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
int initRoutingTable(Router* router) {  //Ŀ������ ���� ��һ��
    int i, j = 0, k;
    RTable *r;
    if (router->dirnetworkCount == 0) return -1;
    router->rtcount = router->dirnetworkCount;  //��ʼ��·�ɱ���Ŀ������ֱ��������
    r = (RTable*)malloc(sizeof(RTable)* router->dirnetworkCount);    //����һ���½ڵ�
    assert(r != NULL);
    printf("\t\t·���� %c ��·�ɱ�\n", router->routerID);
    printf("\t\tĿ������   ����   ��һ��·����\n");
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
* �ġ�·��������·�ɸ�����Ϣ���ھ�·����-����·����Ϣ����-����·�ɸ�����Ϣ���ھ�·����
* updateRoutingTable�����У����ȳ�ʼ��·�ɱ��Ȼ�����ֱ���ھӵ���Ϣ����·�ɱ��֮��ͨ���������µķ�ʽ����·��ѡ���·�ɸ��£�ֱ��û�и���ʱֹͣ��
*********/

RouterType inputRouterID(Network network) {
input:
    printf("\t\t��������Ҫ�������µ�·�������ƣ�");
    char routerID;
    scanf_s("\n%c", &routerID);
    if (getRouterPos(&network, routerID) == -1) {
        printf("\n\t\t·���� not found\n\n");
        goto input;
    }
    else {
        printf("\n\t\t/***�ڽ�·����%c�Ĵ������£�***/\n", routerID);
        return routerID;
    }
}

void triggerUpdate(Network* network, RouterType routerID) {    //·��ѡ���·�ɸ���
    Router* router = NULL;
    router = &network->RL[getRouterPos(network, routerID)];    // ����ָ��·����ID
    int i, j, k;
    Neighbor* neighbor = router->neighbors;
    RTable* rtable;
    int updateFlag = 0;
    while (neighbor != NULL && (updateFlag == 0)) {  // �����ھ�·�ɱ��ϣ�����ÿ���ھӷ��͵�·����Ϣ�����±�·������·�ɱ�
        int updateFlag = 0;
        i = neighbor->neighborID;
        rtable = (RTable*)malloc(sizeof(RTable) * network->RL[i].rtcount);   //������ʱ·�ɱ�����ھ�·�������͵�·�ɱ�
        printf("\t\t·����Ϣ���ڽ�·���� %c -> ·���� %c ����������\n", network->RL[i].routerID, router->routerID);
        printf("\t\tĿ������   ����   ��һ��·����\n"); 
        for (j = 0; j < network->RL[i].rtcount; j++) {  // ·�ɸ���Ŀ������ID���������һ��·����
            rtable[j].distance = network->RL[i].routingTable[j].distance + 1;
            rtable[j].nextHop = network->RL[i].routerID;
            rtable[j].networkID = network->RL[i].routingTable[j].networkID;
            printf("\t\t%6c%7d%7c\n", rtable[j].networkID, rtable[j].distance, rtable[j].nextHop);
        }
        //printf("\n");

        for (int j = 0; j < network->RL[i].rtcount; j++) {  //���ѭ�����ھ�·������·�ɱ��·����Ŀ�����ڲ�ѭ���Ǳ�·������·����Ŀ��   
            int index = j;
            //int* rtnum = (int*)malloc(sizeof(int) * network->RL[i].rtcount);
            for (int k = 0; k < router->rtcount; k++) { //��·�ɱ���ԭ�е�������Ŀ��ÿһ�����յ���·����Ŀ�ȶԣ������Ƿ����µ�������µ�·��
                if (router->routingTable[k].networkID == network->RL[i].routingTable[j].networkID && router->routingTable[k].nextHop == network->RL[i].routingTable[j].nextHop && network->RL[i].routingTable[j].nextHop != router->routerID) {    //����������Ȳ�����һ��Ҳ��ȣ�����һ�����Ǳ�·�������������µ�����Ҳ�����µ�·����·�ɱ����Ѿ�����������缰·���ļ�¼��
                    index = -1;
                    if (router->routingTable[k].distance > rtable[j].distance && rtable[j].distance < INFINITY) {    //�ж��Ƿ���Ҫ�޸�·�ɱ���ԭ�е�·����Ŀ�����¾��룩
                        router->routingTable[k].nextHop = rtable[j].nextHop;
                        router->routingTable[k].distance = rtable[j].distance;
                        router->routingTable[k].networkID = rtable[j].networkID;
                        printf("//�޸ĵ�·��Ϊ��%6c%7d%7c\n",j ,router->routingTable[k].networkID, router->routingTable[k].distance, router->routingTable[k].nextHop);
                    } 
                    break;  //·�ɱ����Ѿ�����������缰·�����˳�����ѭ����ִ����һ�����ѭ��
                }
            }
            if (index != -1) { //��·������·�ɱ�����µ�·����Ŀ��·�ɱ��л�û��������磩,����Ų�ͬ��������̻������ȵ���һ����ͬ
                if (router->rtcount < MAX_ENTRIES) {
                    updateFlag = 0; 
                    for (int k = 0; k < router->rtcount; k++) { //���ѭ�����ھ�·������·�ɱ��·����Ŀ�����ڲ�ѭ���Ǳ�·������·����Ŀ��
                        if (router->routingTable[k].networkID == rtable[index].networkID && router->routingTable[k].nextHop == rtable[index].nextHop && rtable[index].nextHop != router->routerID && router->routingTable[k].distance >= rtable[index].distance) {  
                            updateFlag = 1; //·�ɻ�·
                            printf("\t\t·���� %c ���º��·�ɱ�\n", router->routerID);
                            printf("\t\tĿ������   ����   ��һ��·����\n");
                            for (j = 0; j < router->rtcount; j++) {  // ·�ɸ���Ŀ������ID���������һ��·����
                                printf("\t\t%6c%7d%7c\n", router->routingTable[j].networkID, router->routingTable[j].distance, router->routingTable[j].nextHop);
                            }
                            printf("\n\t\t/***·���� %c ��·���Ѿ�����������Ҫ�����·����Ŀ***/\n\n", router->routerID);
                            return;
                        }
                    }
                    int Flag = 1;
                    for (int k = 0; k < router->rtcount; k++) { //���ѭ�����ھ�·������·�ɱ��·����Ŀ�����ڲ�ѭ���Ǳ�·������·����Ŀ��
                        if(router->routingTable[k].networkID == rtable[index].networkID && router->routingTable[k].distance < rtable[index].distance)
                        Flag = 0;
                    }
                    if (Flag == 1) {
                        RTable* rtable2 = (RTable*)malloc(sizeof(RTable) * 1);   //������ʱ·�ɱ�2�����һ�������ھ�·�����µ�·����Ŀ�������ӵ���·������·�ɱ���
                        rtable2->distance = rtable[index].distance;
                        rtable2->networkID = rtable[index].networkID;
                        rtable2->nextHop = rtable[index].nextHop;
                        router->routingTable[router->rtcount] = *rtable2;
                        //printf("//��ӵ�·��Ϊ��%6c%7d%7c\n", rtable2->networkID, rtable2->distance, rtable2->nextHop);
                        router->rtcount++;
                    }
                    else continue;
                }
                else {
                    printf("\t\t/***·���� %c ��·�ɱ��е�·����Ŀ���Ѵ����ֵ�����������·����Ŀ***/\n\n", router->routerID);
                    return;
                }
            }
        }
        printf("\t\t·���� %c ���º��·�ɱ�\n", router->routerID);
        printf("\t\tĿ������   ����   ��һ��·����\n");
        for (j = 0; j < router->rtcount; j++) {  // ·�ɸ���Ŀ������ID���������һ��·����
            printf("\t\t%6c%7d%7c\n", router->routingTable[j].networkID, router->routingTable[j].distance, router->routingTable[j].nextHop);
        }
        printf("\n");
        neighbor = neighbor->next;
        //Sleep(3000); //ÿ���3000ms�͸���һ��·����
    }
    updateRouter(network, routerID);
}

void updateRouter(Network* network, RouterType routerID) {
    Router* router = NULL;
    router = &network->RL[getRouterPos(network, routerID)];    // ����ָ��·����ID
    int i, j, k;
    Neighbor* neighbor = router->neighbors;
    RTable* rtable;
    int updateFlag = 0;
    char NRouterID = network->RL[neighbor->neighborID].routerID;
    while (neighbor != NULL && NRouterID != router->routerID && updateFlag != 1) {  //����A���ڽ�·����
        NRouterID = network->RL[neighbor->neighborID].routerID;
        printf("\t\t/***�ڽ�·����%c�Ĵ������£�***/\n\n", NRouterID);
        triggerUpdate(network, NRouterID);
        neighbor = neighbor->next;
    }
    /*
    router = &network->RL[getRouterPos(network, routerID)];    // ����ָ��·����ID
    neighbor = router->neighbors;
    updateFlag = 0;
    NRouterID = network->RL[neighbor->neighborID].routerID;
    printf("\t\t/***�ڽ�·����%c�Ĵ������£�/***\n\n", NRouterID);
    triggerUpdate(network, NRouterID);
    */
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

/*********
* �塢��ӡ���н��
*********/

void displayRoutingTable(Network network) { // ��ʾ·�ɱ�����
    //printf("\t\tRouting Table\n");
    //printf("\t\tNetworkID   Distance   Next Hop\n");
    
    for (int i = 0; i < network.routerCount; i++) {
        printf("\t\t·���� % c ��·�ɱ�\n",network.RL[i].routerID);
        printf("\t\tĿ������   ����   ��һ��·����\n");
        for (int j = 0; j < network.RL[i].rtcount;j++) {
            printf("\t\t%6c%7d%7c\n", network.RL[i].routingTable[j].networkID, network.RL[i].routingTable[j].distance, network.RL[i].routingTable[j].nextHop);
        }
        //printf("\n");
    }
}
void displayNetworkSetting(Network network) {   // ��ʾ��������
    int i, j;
    Neighbor* e;
    if (network.routerCount == 0) {
        printf("\t\t��ǰ�����ѳ�ʼ��\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        printf("\n\t\t·����%c ���ڽ�·������Ϊ %d", network.RL[i].routerID, network.RL[i].neighborCount);
        if (network.RL[i].neighborCount > 0) {
            printf("���ֱ�Ϊ");
            for (e = network.RL[i].neighbors; e != NULL; e = e->next) {
                printf(" %c", network.RL[e->neighborID].routerID);  
            }
        }    
    }
    printf("\n\t\t��ǰ������·�������ڽӱ�����\n");
    for (i = 0; i < network.routerCount; i++) {
        printf("\t\t%d: %c", i, network.RL[i].routerID);
        for (e = network.RL[i].neighbors; e != NULL; e = e->next) {
            printf(" -> %d", e->neighborID);  //->
        }
        printf("\n");
    }
    if (network.routerCount ==0) {
        printf("\t\tû��ֱ������\n");
        return;
    }
    for (i = 0; i < network.routerCount; i++) {
        if (network.RL[i].dirnetworkCount == 0) {
                printf("\t\t·����%c û��ֱ������\n", network.RL[i].routerID);
        }
        else {
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
    printf("\t\t**                7�����·������ֱ������      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                8��ɾ��·������ֱ������      **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                9��ɾ����ǰ��������          **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                10��ģ������RIP              **\n");
    printf("\t\t**                                             **\n");
    printf("\t\t**                100���˳�                    **\n");
    printf("\t\t----------------------     ----------------------\n");
    printf("\t\t**************    ���� by @ccy ����    **************\n");
    printf("\t\t-------------------------------------------------\n\n");
    printf("\t\t������ѡ���Ӧ�����֣�");
    int n;
    scanf_s("\n%d", &n);
    switch (n) {
    case 0: system("cls");
        printf("\t\t��ǰ�������ӵ��������£�\n\n");
        displayNetworkSetting(network);
        printf("\t\t���Ѵ�ӡ�굱ǰ�������ã�����������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 1:
        initNetworkFile(&network);
        initNeighborCount(&network);
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
        deleteEdge(&network);
        printf("\n\t\t�������ɾ��·�������Ӳ���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 7:
        insertDirNetwork(&network);
        printf("\n\t\t����������·������ֱ���������������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 8:
        deleteDirNetwork(&network);
        printf("\n\t\t�������ɾ��·������ֱ���������������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 9:
        initNetwork(&network);
        printf("\n\t\t�������ɾ���������ò���������������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 10: system("cls");
        printf("\n\t\tģ��RIP�����������£�\n");
        printf("\t\t��ʼ��·�ɱ�\n");
        for (int i = 0; i < network.routerCount; i++) {
            initRoutingTable(&network.RL[i]);
        }
        triggerUpdate(&network, inputRouterID(network));
        printf("\n\t\t��ӡ���н����\n");
        displayRoutingTable(network);

        printf("\n\t\tģ��RIP��������н�������ϣ�����������س�ʼ����\n");
        printf("\n\t\t");
        system("pause");
        goto start;
        break;
    case 100: printf("\n\t\t���ѳɹ��˳�\n");
        break;
    default: printf("\t\twrong");
    }
}
int main() {
    clock_t begin = clock();    //������̵�����ʱ��
    Network network;
    initNetwork(&network);
    login_func(network);  //ϵͳ�˵�
    clock_t end = clock();
    printf("\n\t\t����ʱ�� %.3lf seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}