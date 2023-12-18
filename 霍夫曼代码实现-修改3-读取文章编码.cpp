#include <stdio.h>
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

void DispHCode(HTNode ht[], HCode hcd[], int n, FILE* file) {
    int i, k;
    int sum = 0, m = 0, j;

    printf("输出哈夫曼编码：\n");
    fprintf(file, "输出哈夫曼编码：\n");

    for (i = 0; i < n; i++) {
        j = 0;

        printf("%c:\t", ht[i].data);
        fprintf(file, "%c:\t", ht[i].data);

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
    fprintf(file, "\n平均长度=%g\n", 1.0 * sum / m);
}

void EncodeFile(HTNode ht[], HCode hcd[], int n, FILE* input, FILE* output) {
    int ch;
    char buffer[8];
    int bufferIndex = 0;

    while ((ch = fgetc(input)) != EOF) {
        int index = ch;

        for (int i = hcd[index].start; i <= n; i++) {
            buffer[bufferIndex++] = hcd[index].cd[i];

            if (bufferIndex == 8) {
                unsigned char byte = 0;

                for (int j = 0; j < 8; j++)
                    byte = (byte << 1) | (buffer[j] - '0');

                fputc(byte, output);

                bufferIndex = 0;
            }
        }
    }

    if (bufferIndex > 0) {
        while (bufferIndex < 8)
            buffer[bufferIndex++] = '0';

        unsigned char byte = 0;

        for (int i = 0; i < 8; i++)
            byte = (byte << 1) | (buffer[i] - '0');

        fputc(byte, output);
    }
}

int main() {
    char inputFilename[100], outputFilename[100];

    printf("请输入要打开的文件名称：");
    scanf("%s", inputFilename);

    FILE* inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        printf("无法打开文件！");
        return -1;
    }

    int fnum[N] = { 0 };
    int ch;

    while ((ch = fgetc(inputFile)) != EOF)
        fnum[ch]++;

    int n = 0;
    for (int i = 0; i < N; i++) {
        if (fnum[i] > 0)
            n++;
    }

    HTNode ht[M];
    HCode hcd[N];
    int idx = 0;

    for (int i = 0; i < N; i++) {
        if (fnum[i] > 0) {
            ht[idx].data = (char)i;
            ht[idx].weight = fnum[i];
            idx++;
        }
    }

    CreateHT(ht, n);
    CreateHCode(ht, hcd, n);

    printf("请输入要保存的文件名称：");
    scanf("%s", outputFilename);

    FILE* outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("无法打开文件！");
        return -1;
    }

    EncodeFile(ht, hcd, n, inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    printf("替换完成！");

    return 0;
}
