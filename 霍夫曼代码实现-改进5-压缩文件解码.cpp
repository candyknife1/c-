#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define M 2 * N - 1

typedef struct {
    char data;
    int weight;
    int parent;
    int lchild;
    int rchild;
} HTNode;

typedef struct {
    char cd[N];
    int start;
} HCode;

void CreateHT(HTNode ht[], int n) {
    int i, k, lnode, rnode;
    int min1, min2;

    for (i = 0; i < 2 * n - 1; i++)
        ht[i].parent = ht[i].lchild = ht[i].rchild = -1;

    for (i = n; i < 2 * n - 1; i++) {
        min1 = min2 = 32767;
        lnode = rnode = -1;

        for (k = 0; k <= i - 1; k++) {
            if (ht[k].parent == -1) {
                if (ht[k].weight < min1) {
                    min2 = min1;
                    rnode = lnode;
                    min1 = ht[k].weight;
                    lnode = k;
                }
                else if (ht[k].weight < min2) {
                    min2 = ht[k].weight;
                    rnode = k;
                }
            }
        }

        ht[lnode].parent = i;
        ht[rnode].parent = i;
        ht[i].weight = ht[lnode].weight + ht[rnode].weight;
        ht[i].lchild = lnode;
        ht[i].rchild = rnode;
    }
}

void CreateHCode(HTNode ht[], HCode hcd[], int n) {
    int i, f, c;
    HCode hc;

    for (i = 0; i < n; i++) {
        hc.start = n;
        c = i;
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

void DispHCode(HTNode ht[], HCode hcd[], int n) {
    int i, k;
    int sum = 0, m = 0, j;

    printf("输出哈夫曼编码：\n");

    for (i = 0; i < n; i++) {
        j = 0;
        printf("%c:\t", ht[i].data);

        for (k = hcd[i].start; k <= n; k++) {
            printf("%c", hcd[i].cd[k]);
            j++;
        }

        printf("\n");
        m += ht[i].weight;
        sum += ht[i].weight * j;
    }
}

void replaceFileContentWithHuffmanCode(char input[], char output[], HTNode ht[], HCode hcd[], int n) {
    FILE* fin = fopen(input, "r");
    FILE* fout = fopen(output, "wb");

    if (fin == NULL || fout == NULL) {
        printf("无法打开文件！！");
        return;
    }

    int ch;
    char buffer = 0;
    int count = 0;

    while ((ch = fgetc(fin)) != EOF) {
        for (int i = 0; i < n; i++) {
            if (ht[i].data == ch) {
                for (int j = hcd[i].start; j <= n; j++) {
                    buffer <<= 1;
                    if (hcd[i].cd[j] == '1')
                        buffer |= 1;

                    count++;

                    if (count == 8) {
                        fputc(buffer, fout);
                        buffer = 0;
                        count = 0;
                    }
                }
            }
        }
    }

    if (count > 0) {
        buffer <<= (8 - count);
        fputc(buffer, fout);
    }

    fclose(fin);
    fclose(fout);
}

void decodeHuffmanFile(char input[], char output[], HTNode ht[], HCode hcd[], int n) {
    FILE* fin = fopen(input, "rb");
    FILE* fout = fopen(output, "w");

    if (fin == NULL || fout == NULL) {
        printf("无法打开文件！！");
        return;
    }

    int ch;

    int p = 2 * n - 2;
    int idx = p;

    while ((ch = fgetc(fin)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            int bit = (ch >> i) & 1;

            if (bit == 1)
                idx = ht[idx].rchild;
            else
                idx = ht[idx].lchild;

            if (ht[idx].lchild == -1) {
                fputc(ht[idx].data, fout);
                idx = p;
            }
        }
    }

    fclose(fin);
    fclose(fout);
}


void printHuffmanCompressedDataToConsole(char input[], HTNode ht[], HCode hcd[], int n) {
    FILE* fin = fopen(input, "r");

    if (fin == NULL) {
        printf("无法打开文件！！");
        return;
    }

    int ch;

    while ((ch = fgetc(fin)) != EOF) {
        for (int i = 0; i < n; i++) {
            if (ht[i].data == ch) {
                for (int j = hcd[i].start; j <= n; j++) {
                    printf("%c", hcd[i].cd[j]);
                }
            }
        }
    }

    fclose(fin);
}


int main() {
    char filename[100];
    printf("请输入要打开的文件名称：");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("无法打开文件！！");
        return -1;
    }

    int fnum[N] = { 0 };
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        fnum[ch]++;
    }

    int n = 0;

    for (int i = 0; i < N; i++) {
        if (fnum[i] > 0) {
            n++;
        }
    }

    HTNode ht[M];
    HCode hcd[N];

    int idx = 0;
    int i;

    for (i = 0; i < N; i++) {
        if (fnum[i] > 0) {
            ht[idx].data = (char)i;
            ht[idx].weight = fnum[i];
            idx++;
        }
    }

    CreateHT(ht, n);
    CreateHCode(ht, hcd, n);

    DispHCode(ht, hcd, n);

    char outputFilename[100];
    printf("请输入要保存的文件名称：");
    scanf("%s", outputFilename);

    replaceFileContentWithHuffmanCode(filename, outputFilename, ht, hcd, n);

    printf("替换成功！\n");
    printf("哈夫曼编码结果为：\n");
    printHuffmanCompressedDataToConsole(filename, ht, hcd, n);
    int choice;
    printf("\n是否进行解码输出？（1表示是，0表示否）：");
    scanf("%d", &choice);

    if (choice == 1) {
        char decodeOutputFilename[100];
        printf("请输入要保存解码后文件的名称：");
        scanf("%s", decodeOutputFilename);

        decodeHuffmanFile(outputFilename, decodeOutputFilename, ht, hcd, n);

        printf("解码成功！");
    }

    return 0;
}

