#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 50
#define MaxValue 0//表示顶点与顶点之间的连接状况

typedef char VertexType;//顶点的数据类型
typedef int EdgeType;//边的数据类型
/*
 * 用邻接矩阵去存储图的结构
 */
typedef struct {
    VertexType vexs[MaxVertexNum];//存储顶点的数组
    EdgeType edges[MaxVertexNum][MaxVertexNum];//邻接矩阵
    int vexnum, edgenum;//图中的顶点数和边的数量
} MGraph;

typedef struct SeqQueue {
    int data[MaxVertexNum];
    int front, rear;
} PSeqQueue;

//第二种方法实现广度遍历。用一个前继节点数组来存储
typedef struct node {
    VertexType vex;
    struct node *preNode[MaxVertexNum];
} Node;

int visited[MaxVertexNum];//表示这个点的状态

Node nodes[MaxVertexNum];//BFS节点数组

/*
 * 队列的一系列操作
 */
PSeqQueue Init_SeqQueue();//初始化队列

int Empty_SeqQueue(PSeqQueue Q);//判断队列是否为空

int In_SeqQueue(PSeqQueue Q, int x);//入队

int Out_SeqQueue(PSeqQueue Q, int *x);//出队

void Destroy_SeqQueue(PSeqQueue *Q);//摧毁队列

void createGraph(MGraph *G);//初始化图的数据

void BFSSearch(MGraph *G);

void DFSSearch(MGraph *G);

void BFSByQueue(MGraph *G, int start);

void BFSByArray(MGraph *G);

void DFS(MGraph *G, int start);

void PrintNode(char c);

int isEnd(Node node);//判断是否是最后一个节点,返回1表示不是最后一个节点，返回0表示是最后一个节点

int ChartoPoint(char vex, MGraph *G);//通过传一个vex返回对应的顶点索引


int main() {
    MGraph G;
    createGraph(&G);
    printf("深度优先遍历结果\n");
    DFSSearch(&G);
    printf("\n使用队列实现的广度优先遍历结果\n");
    BFSSearch(&G);
    printf("\n使用前继数组实现的广度优先遍历结果\n");
    BFSByArray(&G);
    return 0;
}

void createGraph(MGraph *G) {
    int i, j, k, l, count;
    char start, end;//边的起始定点
    FILE *fp;
    if ((fp = fopen("D://thirddata//data2.txt", "r")) == NULL) {
        printf("打开失败!");
        exit(0);
    } else {
        fscanf(fp, "%d %d", &G->vexnum, &G->edgenum);
        //初始化邻接矩阵
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
        //读取数据
        for (k = 0; k < G->edgenum; k++) {
            fscanf(fp, " %c %c", &start, &end);
            for (i = 0; start != G->vexs[i]; i++);//查找起点
            for (j = 0; end != G->vexs[j]; j++);//查找终点
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
    printf("节点数：%d\n", G->vexnum);
    for (int m = 0; m < G->vexnum; ++m) {Sp
        PrintNode(G->vexs[m]);
    }
    printf("\n边数： %d\n", G->edgenum);
}

//深度优先遍历（递归）
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
        //出队列
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
        printf("队满！！");
        return -1;
    } else {
        Q.rear = (Q.rear + 1) % MaxVertexNum;
        Q.data[Q.rear] = x;
        return 1;
    }
}

int Out_SeqQueue(PSeqQueue Q, int *x) {
    if (Empty_SeqQueue(Q)) {
        printf("队空！");
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

//判断是否是最后一个节点
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
                    //将当前前继节点的索引找到
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
            printf("1.重庆\t");
            break;
        case 'B':
            printf("2.北京\t");
            break;
        case 'C':
            printf("3.成都\t");
            break;
        case 'D':
            printf("4.上海\t");
            break;
        case 'E':
            printf("5.深圳\t");
            break;
        case 'F':
            printf("6.杭州\t");
            break;
        case 'G':
            printf("7.广州\t");
            break;
        case 'H':
            printf("8.武汉\t");
            break;
        case 'I':
            printf("9.天津\t");
            break;
        case 'J':
            printf("10.南京\t");
            break;
    }
}

