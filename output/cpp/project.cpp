/*
#include <stdio.h>
#include <windows.h>
#include <time.h>
typedef struct RoutTable { //����·�ɱ� 
	unsigned char tn; 	   //Ŀ�������ַ 
	unsigned char d; 	   //���� 
	char next;			   //��һ��·������
}RTable;

void insertTable(RTable rt[], int* count, unsigned char tn, unsigned char d, char next);
void init(RTable rt[], int* count);
void prt(RTable rt[], int count);
void updateRIP(RTable rt[], int* count, RTable rtu[], int* count2);
void receiveRIP(RTable rt[], int* count);

int main() {
	RTable rt[100], rtu[100];
	//rt��ʾ��Ҫά���ı���·������·�ɱ������100������Ŀ 
	//rtu��ʾ���յ�������·������·�ɱ� 
	int count = 0, count2 = 0; //�ֱ��ʾrt��rtu����·����ʵ�ʵ�·���� 
	init(rt, &count); //��·����rt���г�ʼ����count���õ�ַ���ݷ�ʽ 
	prt(rt, count); //��ӡ·������rt 
	while (1) {
		Sleep(3000); //ÿ���3000ms�͸���һ��·���� 
		receiveRIP(rtu, &count2); //�յ���������·������·�ɱ� 
		printf("���յ���·����Ϣ:\n");
		prt(rtu, count2);
		updateRIP(rt, &count, rtu, &count2);
		printf("���º��·����Ϣ:\n");
		prt(rt, count);
	}
	return 0;
}
void init(RTable rt[], int* count) {
	insertTable(rt, count, 1, 1, '-'); //��rt����һ��Ŀ������Ϊ1������Ϊ1����һ�����ؽ�����·�ɱ���Ŀ 
	insertTable(rt, count, 4, 1, '-');
}
void insertTable(RTable rt[], int* count, unsigned char tn,
	unsigned char d, char next) {
	rt[*count].tn = tn;
	rt[*count].d = d;
	rt[*count].next = next;
	(*count)++; //·�ɱ������һ 
}
void prt(RTable rt[], int count) {
	unsigned char i;
	printf("Ŀ������ ���� ��һ��    \n");
	for (i = 0; i < count; i++) {
		printf("%6d%6d%6c\n", rt[i].tn, rt[i].d, rt[i].next);
	}
}
void receiveRIP(RTable rt[], int* count) {
	int i, nx;
	srand((unsigned)time(NULL));//ʱ������ 
	*count = rand() % 4 + 1;    //·�ɱ�������������ɵ� 
	nx = rand() % 5 + 65;       //����һ��·���������ģ������Χ'A'-'E'
	for (i = 0; i < *count; i++) {
		Sleep(500);
		srand((unsigned)time(NULL));
		rt[i].tn = rand() % 9 + 1; //�������1-9֮���Ŀ������ 
		Sleep(500);
		srand((unsigned)time(NULL));
		rt[i].d = 1; //���ɾ���Ϊ1 
		rt[i].next = nx; //�����յ�����·�ɱ�����һ��next����Ϊ������·����nx  
	}
}
void updateRIP(RTable rt[], int* count, RTable rtu[], int* count2) {
	int i, j;
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