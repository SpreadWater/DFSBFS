#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 50
#define MaxValue 0//��ʾ�����붥��֮�������״��

typedef char VertexType;//�������������
typedef int EdgeType;//�ߵ���������
/*
 * ���ڽӾ���ȥ�洢ͼ�Ľṹ
 */
typedef struct {
    VertexType vexs[MaxVertexNum];//�洢���������
    EdgeType edges[MaxVertexNum][MaxVertexNum];//�ڽӾ���
    int vexnum, edgenum;//ͼ�еĶ������ͱߵ�����
} MGraph;

typedef struct SeqQueue {
    int data[MaxVertexNum];
    int front, rear;
} PSeqQueue;

//�ڶ��ַ���ʵ�ֹ�ȱ�������һ��ǰ�̽ڵ��������洢
typedef struct node {
    VertexType vex;
    struct node *preNode[MaxVertexNum];
} Node;

int visited[MaxVertexNum];//��ʾ������״̬

Node nodes[MaxVertexNum];//BFS�ڵ�����

/*
 * ���е�һϵ�в���
 */
PSeqQueue Init_SeqQueue();//��ʼ������

int Empty_SeqQueue(PSeqQueue Q);//�ж϶����Ƿ�Ϊ��

int In_SeqQueue(PSeqQueue Q, int x);//���

int Out_SeqQueue(PSeqQueue Q, int *x);//����

void Destroy_SeqQueue(PSeqQueue *Q);//�ݻٶ���

void createGraph(MGraph *G);//��ʼ��ͼ������

void BFSSearch(MGraph *G);

void DFSSearch(MGraph *G);

void BFSByQueue(MGraph *G, int start);

void BFSByArray(MGraph *G);

void DFS(MGraph *G, int start);

void PrintNode(char c);

int isEnd(Node node);//�ж��Ƿ������һ���ڵ�,����1��ʾ�������һ���ڵ㣬����0��ʾ�����һ���ڵ�

int ChartoPoint(char vex, MGraph *G);//ͨ����һ��vex���ض�Ӧ�Ķ�������


int main() {
    MGraph G;
    createGraph(&G);
    printf("������ȱ������\n");
    DFSSearch(&G);
    printf("\nʹ�ö���ʵ�ֵĹ�����ȱ������\n");
    BFSSearch(&G);
    printf("\nʹ��ǰ������ʵ�ֵĹ�����ȱ������\n");
    BFSByArray(&G);
    return 0;
}

void createGraph(MGraph *G) {
    int i, j, k, l, count;
    char start, end;//�ߵ���ʼ����
    FILE *fp;
    if ((fp = fopen("D://thirddata//data2.txt", "r")) == NULL) {
        printf("��ʧ��!");
        exit(0);
    } else {
        fscanf(fp, "%d %d", &G->vexnum, &G->edgenum);
        //��ʼ���ڽӾ���
        for (i = 0; i < G->vexnum; ++i) {
            for (j = 0; j < G->vexnum; ++j) {
                G->edges[i][j] = MaxValue;
            }
        }
        for (l = 0; l < G->vexnum; l++) {
            char data;
            fscanf(fp, " %c\n", &data);
            G->vexs[l] = data;
            nodes[l].vex = data;
        }
        //��ȡ����
        for (k = 0; k < G->edgenum; k++) {
            fscanf(fp, " %c %c", &start, &end);
            for (i = 0; start != G->vexs[i]; i++);//�������
            for (j = 0; end != G->vexs[j]; j++);//�����յ�
            G->edges[j][i] = 1;
            G->edges[i][j] = 1;
            for (int m = 0; m < MaxVertexNum; ++m) {
                if (nodes[i].preNode[m] == NULL) {
                    nodes[i].preNode[m] = &nodes[j];
                    break;
                }
            }
        }
    }
    printf("�ڵ�����%d\n", G->vexnum);
    for (int m = 0; m < G->vexnum; ++m) {Sp
        PrintNode(G->vexs[m]);
    }
    printf("\n������ %d\n", G->edgenum);
}

//������ȱ������ݹ飩
void DFS(MGraph *G, int start) {
    int j;
    PrintNode(G->vexs[start]);
    visited[start] = 1;
    for (j = 0; j < G->vexnum; ++j) {
        if (G->edges[start][j] == 1 && !visited[j]) {
            DFS(G, j);
        }
    }
}

void DFSSearch(MGraph *G) {
    int i;
    for (i = 0; i < G->vexnum; i++)
        visited[i] = 0;
    for (i = 0; i < G->vexnum; i++) {
        if (!visited[i])
            DFS(G, i);
    }
}

void BFSByQueue(MGraph *G, int start) {
    int i, j;
    PSeqQueue Q;
    Q = Init_SeqQueue();
    PrintNode(G->vexs[start]);

    visited[start] = 1;
    In_SeqQueue(Q, start);
    while (!Empty_SeqQueue(Q)) {
        //������
        Out_SeqQueue(Q, &i);
        for (j = 0; j < G->vexnum; ++j) {
            if (G->edges[i][j] == 1 && !visited[j]) {
                PrintNode(G->vexs[j]);
                visited[j] = 1;
                In_SeqQueue(Q, j);
            }
        }
    }
    Destroy_SeqQueue(&Q);
}

void BFSSearch(MGraph *G) {
    int i, v;
    for (v = 0; v < G->vexnum; v++)
        visited[v] = 0;
    for (i = 0; i < G->vexnum; i++) {
        if (!visited[i]) {
            BFSByQueue(G, i);
        }
    }
}

PSeqQueue Init_SeqQueue() {
    PSeqQueue *Q;
    Q = (PSeqQueue *) malloc(sizeof(struct SeqQueue));
    if (Q) {
        Q->front = 0;
        Q->rear = 0;
    }
    return *Q;
}

int Empty_SeqQueue(PSeqQueue Q) {
    if (&Q && Q.front == Q.rear) {
        return 1;
    } else {
        return 0;
    }
}

int In_SeqQueue(PSeqQueue Q, int x) {
    if ((Q.rear + 1) % MaxVertexNum == Q.front) {
        printf("��������");
        return -1;
    } else {
        Q.rear = (Q.rear + 1) % MaxVertexNum;
        Q.data[Q.rear] = x;
        return 1;
    }
}

int Out_SeqQueue(PSeqQueue Q, int *x) {
    if (Empty_SeqQueue(Q)) {
        printf("�ӿգ�");
        return -1;
    } else {
        Q.front = (Q.front + 1) % MaxVertexNum;
        *x = Q.data[Q.front];
        return 1;
    }
}

void Destroy_SeqQueue(PSeqQueue *Q) {
    if (Q)
        free(Q);
    Q = NULL;
}

//�ж��Ƿ������һ���ڵ�
int isEnd(Node node) {
    int result = 0;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (node.preNode[i] != NULL) {
            result = 1;
            break;
        }
    }
    return result;
}

int ChartoPoint(char vex, MGraph *G) {
    for (int i = 0; i < G->vexnum; ++i) {
        if (vex == G->vexs[i]) {
            return i;
        }
    }
}

void BFSByArray(MGraph *G) {
    for (int i = 0; i < G->vexnum; i++)
        visited[i] = 0;

    for (int i = 0; i < G->vexnum; ++i) {
        if (!isEnd(nodes[i]) && !visited[i]) {
            PrintNode(nodes[i].vex);
            visited[i] = 1;
            return;
        } else {
            if (!visited[i]){
                PrintNode(nodes[i].vex);
                visited[i] = 1;
            }
            for (int j = 0; j < G->vexnum; ++j) {
                if (nodes[i].preNode[j] == NULL) {
                    break;
                } else {
                    //����ǰǰ�̽ڵ�������ҵ�
                    int position = ChartoPoint(nodes[i].preNode[j]->vex, G);
                    if (!visited[position]) {
                        PrintNode(nodes[i].preNode[j]->vex);
                        visited[position] = 1;
                    }
                }
            }
        }
    }

}

void PrintNode(char c) {
    switch (c) {
        case 'A':
            printf("1.����\t");
            break;
        case 'B':
            printf("2.����\t");
            break;
        case 'C':
            printf("3.�ɶ�\t");
            break;
        case 'D':
            printf("4.�Ϻ�\t");
            break;
        case 'E':
            printf("5.����\t");
            break;
        case 'F':
            printf("6.����\t");
            break;
        case 'G':
            printf("7.����\t");
            break;
        case 'H':
            printf("8.�人\t");
            break;
        case 'I':
            printf("9.���\t");
            break;
        case 'J':
            printf("10.�Ͼ�\t");
            break;
    }
}

