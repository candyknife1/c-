#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
int main() {
	FILE* file;
	char filename[100];
	char buffer[1000];

	printf("请输入要打开的文件名：");
	scanf("%s", filename);

	file = fopen(filename, "r");
	if (file == NULL) {
		printf("无法打开文件\n");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}
	fclose(file);
	getchar();
	getchar();
	return 0;
}