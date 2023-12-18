#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <iostream>;
int main() {
	FILE* pf;
	char filename[100];
	char content[1000];
	char sign[] = "###";
	int a=0;
	printf("请输入要打开并写入的文件：");
	scanf("%s", filename);
	getchar();
	pf = fopen(filename, "w");
	if (pf == NULL) {
		perror("fopen:");
		return 1;
	}
	printf("1.主动输入\n2.机器输入\n请选择输入方式：");
abc:scanf("%d", &a);
	getchar();
	if (a == 1) {
		printf("请输入要写入的文件内容（输入 ### 结束）");
		while (1) {
			scanf("%[^\n]s",content);
			getchar();
			if (strcmp(content, sign) == 0)break;
			fprintf(pf, " %s\n", content);
		}
	}
	else if (a == 2) {
		char ch = 0;
		for (ch = 'a'; ch <= 'z'; ch++)
		{
			fputc(ch, pf);
		}
	}
	else {
		printf("输入有误！！！请重新输入：");
			goto abc;
	}
		fclose(pf);
		printf("内容已写入");
		pf = NULL;
		return 0;
}

