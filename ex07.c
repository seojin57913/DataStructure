#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define V 10        
#define E 20        
#define INF 999999  

void dijkstra(int graph[V][V], int start, int dist[V]) {
    int visited[V] = {0};
    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[start] = 0;
    for (int i = 0; i < V - 1; i++) {
        int min = INF, u = -1;
        for (int j = 0; j < V; j++)
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        if (u == -1)
            break;
        visited[u] = 1;
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }
}

int main() {
    int graph[V][V] = {0};
    srand((unsigned int)time(NULL));
    int edges = 0;
    while (edges < E) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && graph[u][v] == 0) {
            int w = rand() % 9 + 1; 
            graph[u][v] = graph[v][u] = w;
            edges++;
        }
    }
    printf("=== 생성된 그래프 (인접 행렬) ===\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("=== 모든 정점 쌍의 최단거리 (정점 1~10) ===\n");
    for (int i = 0; i < V; i++) {
        int dist[V];
        dijkstra(graph, i, dist);
        for (int j = i + 1; j < V; j++) {
            if (dist[j] == INF)
                printf("(%d, %d): INF\n", i + 1, j + 1);
            else
                printf("(%d, %d): %d\n", i + 1, j + 1, dist[j]);
        }
    }
    return 0;
}
