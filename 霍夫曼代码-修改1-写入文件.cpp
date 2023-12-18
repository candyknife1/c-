#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#define N 50
#define M 2*N-1 
typedef struct {
	char data[5];
		int weight;
		int parent;
		int lchild;
		int rchild;
		}HTNode;
typedef struct {
	char cd[N];
	int start;
}HCode;
void CreateHT(HTNode ht[], int n)
{
	int i, k, lnode, rnode;
	int min1, min2;
	for (i = 0; i < 2 * n - 1; i++)
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	for (i = n; i < 2 * n - 1; i++) {
		min1 = min2 = 32767;
		lnode = rnode = -1;
		for(k=0;k<=i-1;k++)
			if (ht[k].parent == -1) {
				if (ht[k].weight < min1)
				{
					min2 = min1;
					rnode = lnode;
					min1 = ht[k].weight; lnode = k;
				}
				else if (ht[k].weight < min2)
				{
					min2 = ht[k].weight;
					rnode = k;
				}
			}
		ht[lnode].parent = i; ht[rnode].parent = i;
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode; ht[i].rchild = rnode;
	}                                                                                                                                        
}
void CreateHCode(HTNode ht[], HCode hcd[], int n) {
	int i, f, c;
	HCode hc;
	for (i = 0; i < n; i++) {
		hc.start = n; c = i;
		f = ht[i].parent;
		while (f != -1) {
			if (ht[f].lchild == c)
				hc.cd[hc.start--] = '0';
			else
				hc.cd[hc.start--] = '1';
			c = f;
			f = ht[f].parent;
		}
		hc.start++;
		hcd[i] = hc;
	}
}

void DispHCode(HTNode ht[], HCode hcd[], int n,FILE *file) {
	int i, k;
	int sum = 0, m = 0, j;
	printf("输出哈夫曼编码：\n");
	fprintf(file, "输出哈夫曼编码：\n");
	for (i = 0; i < n; i++) {
		j = 0;
		printf("%14s:\t", ht[i].data);
		fprintf(file, "%14s:\t", ht[i].data);
		for (k = hcd[i].start; k <= n; k++) {
			printf("%c", hcd[i].cd[k]);
			fprintf(file, "%c", hcd[i].cd[k]);
			j++;
		}
		printf("\n");
		fprintf(file, "\n");
		m += ht[i].weight;
		sum += ht[i].weight * j;
	}
	printf("\n平均长度=%g\n", 1.0 * sum / m);
	fprintf(file,"\n平均长度=%g\n", 1.0 * sum / m);
}
int main() {
	int n, i;
	printf("请输入要霍夫曼编码的字符串个数：");
	scanf("%d", &n);

	HTNode ht[M];
	HCode hcd[N];

	char str[N][5];
	int fnum[N];

	for (i = 0; i < n; i++) {
		printf("请输入第 %d 个字符串和对应的权值（例如：“and 36”）：", i + 1);
		scanf("%s %d", str[i], &fnum[i]);
	}

	for (i = 0; i < n; i++) {
		strcpy(ht[i].data, str[i]);
		ht[i].weight = fnum[i];
	}

	CreateHT(ht, n);
	CreateHCode(ht, hcd, n);

	FILE* fp;
	char filename[100];
	printf("请输入要保存的文件名称：");
	scanf("%s", filename);
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("无法打开文件！！！");
		return -1;
	}
	DispHCode(ht, hcd, n, fp);
	fclose(fp);

	return 0;
}
