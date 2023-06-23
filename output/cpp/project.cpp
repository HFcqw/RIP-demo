/*
#include <stdio.h>
#include <windows.h>
#include <time.h>
typedef struct RoutTable { //定义路由表 
	unsigned char tn; 	   //目的网络地址 
	unsigned char d; 	   //距离 
	char next;			   //下一跳路由名字
}RTable;

void insertTable(RTable rt[], int* count, unsigned char tn, unsigned char d, char next);
void init(RTable rt[], int* count);
void prt(RTable rt[], int count);
void updateRIP(RTable rt[], int* count, RTable rtu[], int* count2);
void receiveRIP(RTable rt[], int* count);

int main() {
	RTable rt[100], rtu[100];
	//rt表示需要维护的本地路由器，路由表最多有100条表项目 
	//rtu表示接收到的相邻路由器的路由表 
	int count = 0, count2 = 0; //分别表示rt和rtu两个路由器实际的路由数 
	init(rt, &count); //对路由器rt进行初始化，count采用地址传递方式 
	prt(rt, count); //打印路由器表rt 
	while (1) {
		Sleep(3000); //每间隔3000ms就更新一次路由器 
		receiveRIP(rtu, &count2); //收到来自相邻路由器的路由表 
		printf("接收到的路由信息:\n");
		prt(rtu, count2);
		updateRIP(rt, &count, rtu, &count2);
		printf("更新后的路由信息:\n");
		prt(rt, count);
	}
	return 0;
}
void init(RTable rt[], int* count) {
	insertTable(rt, count, 1, 1, '-'); //向rt插入一个目的网络为1，距离为1，下一跳本地交付的路由表项目 
	insertTable(rt, count, 4, 1, '-');
}
void insertTable(RTable rt[], int* count, unsigned char tn,
	unsigned char d, char next) {
	rt[*count].tn = tn;
	rt[*count].d = d;
	rt[*count].next = next;
	(*count)++; //路由表个数加一 
}
void prt(RTable rt[], int count) {
	unsigned char i;
	printf("目的网络 距离 下一跳    \n");
	for (i = 0; i < count; i++) {
		printf("%6d%6d%6c\n", rt[i].tn, rt[i].d, rt[i].next);
	}
}
void receiveRIP(RTable rt[], int* count) {
	int i, nx;
	srand((unsigned)time(NULL));//时间种子 
	*count = rand() % 4 + 1;    //路由表数量是随机生成的 
	nx = rand() % 5 + 65;       //从哪一个路由器发来的，随机范围'A'-'E'
	for (i = 0; i < *count; i++) {
		Sleep(500);
		srand((unsigned)time(NULL));
		rt[i].tn = rand() % 9 + 1; //随机生成1-9之间的目标网络 
		Sleep(500);
		srand((unsigned)time(NULL));
		rt[i].d = 1; //生成距离为1 
		rt[i].next = nx; //将接收的相邻路由表中下一跳next设置为两相邻路由器nx  
	}
}
void updateRIP(RTable rt[], int* count, RTable rtu[], int* count2) {
	int i, j;
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