#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int** matrix;
  int num_vertices;
  long long comparisons;
} AdjacencyMatrix;

AdjacencyMatrix* createAdjacencyMatrix(int num_vertices) {
  AdjacencyMatrix* graph = (AdjacencyMatrix*)malloc(sizeof(AdjacencyMatrix));
  graph->num_vertices = num_vertices;
  graph->comparisons = 0;

  graph->matrix = (int**)malloc(num_vertices * sizeof(int*));
  for (int i = 0; i < num_vertices; i++) {
      graph->matrix[i] = (int*)calloc(num_vertices, sizeof(int));
  }
  return graph;
}

void addEdgeMatrix(AdjacencyMatrix* graph, int u, int v) {
  graph->comparisons = 0;
  if (u >= 0 && u < graph->num_vertices && v >= 0 && v < graph->num_vertices) {
    graph->comparisons += 2;
    graph->matrix[u][v] = 1;
    graph->matrix[v][u] = 1;
    graph->comparisons += 2; 
  }
}

void removeEdgeMatrix(AdjacencyMatrix* graph, int u, int v) {
  graph->comparisons = 0;
  if (u >= 0 && u < graph->num_vertices && v >= 0 && v < graph->num_vertices) {
    graph->comparisons += 2;
    graph->matrix[u][v] = 0;
    graph->matrix[v][u] = 0;
    graph->comparisons += 2;
  }
}

int isConnectedMatrix(AdjacencyMatrix* graph, int u, int v) {
  graph->comparisons = 0;
  if (u >= 0 && u < graph->num_vertices && v >= 0 && v < graph->num_vertices) {
    graph->comparisons += 2;  
    graph->comparisons += 1;  
    return graph->matrix[u][v] == 1;
  }
  return 0;
}

int* getAdjacentVerticesMatrix(AdjacencyMatrix* graph, int u, int* count) {
  graph->comparisons = 0;
  int* adjacent = (int*)malloc(graph->num_vertices * sizeof(int));
  *count = 0;

  if (u >= 0 && u < graph->num_vertices) {
    graph->comparisons += 1;
    for (int v = 0; v < graph->num_vertices; v++) {
      graph->comparisons += 1; 
      if (graph->matrix[u][v] == 1) {
        graph->comparisons += 1;
        adjacent[(*count)++] = v;
      }
    }
  }
  return adjacent;
}

long long getMemoryUsageMatrix(AdjacencyMatrix* graph) {
  return (long long)graph->num_vertices * graph->num_vertices * sizeof(int);
}

void freeAdjacencyMatrix(AdjacencyMatrix* graph) {
  for (int i = 0; i < graph->num_vertices; i++) {
    free(graph->matrix[i]);
  }
  free(graph->matrix);
  free(graph);
}

typedef struct Node {
  int vertex;
  struct Node* next;
} Node;

typedef struct {
  Node** adjacency_list;
  int num_vertices;
  long long comparisons;
} AdjacencyList;

AdjacencyList* createAdjacencyList(int num_vertices) {
  AdjacencyList* graph = (AdjacencyList*)malloc(sizeof(AdjacencyList));
  graph->num_vertices = num_vertices;
  graph->comparisons = 0;
  graph->adjacency_list = (Node**)calloc(num_vertices, sizeof(Node*));
  return graph;
}

int isVertexInList(Node* head, int v, long long* comparisons) {
  while (head != NULL) {
    (*comparisons)++;
    if (head->vertex == v) {
      return 1;
    }
    head = head->next;
  }
  return 0;
}

void addEdgeList(AdjacencyList* graph, int u, int v) {
  graph->comparisons = 0;
  if (u >= 0 && u < graph->num_vertices && v >= 0 && v < graph->num_vertices) {
    graph->comparisons += 2;
    if (!isVertexInList(graph->adjacency_list[u], v, &graph->comparisons)) {
      graph->comparisons += 1;
      Node* newNode = (Node*)malloc(sizeof(Node));
      newNode->vertex = v;
      newNode->next = graph->adjacency_list[u];
      graph->adjacency_list[u] = newNode;
      graph->comparisons += 2;
    }
    if (!isVertexInList(graph->adjacency_list[v], u, &graph->comparisons)) {
      graph->comparisons += 1;
      Node* newNode = (Node*)malloc(sizeof(Node));
      newNode->vertex = u;
      newNode->next = graph->adjacency_list[v];
      graph->adjacency_list[v] = newNode;
      graph->comparisons += 2;
    }
  }
}

int isConnectedList(AdjacencyList* graph, int u, int v) {
  graph->comparisons = 0;
  if (u >= 0 && u < graph->num_vertices && v >= 0 && v < graph->num_vertices) {
    graph->comparisons += 2;
    return isVertexInList(graph->adjacency_list[u], v, &graph->comparisons);
  }
  return 0;
}

int* getAdjacentVerticesList(AdjacencyList* graph, int u, int* count) {
  graph->comparisons = 0;
  *count = 0;
  Node* current = graph->adjacency_list[u];
  while (current != NULL) {
    graph->comparisons += 1;
    (*count)++;
    current = current->next;
  }
  int* adjacent = (int*)malloc(*count * sizeof(int));
  *count = 0;
  current = graph->adjacency_list[u];
  while (current != NULL) {
    graph->comparisons += 1;
    adjacent[(*count)++] = current->vertex;
    current = current->next;
  }
  return adjacent;
}

long long getMemoryUsageList(AdjacencyList* graph) {
  long long total_edges = 0;
  for (int i = 0; i < graph->num_vertices; i++) {
    Node* current = graph->adjacency_list[i];
    while (current != NULL) {
        total_edges++;
        current = current->next;
    }
  }
  return 8 * graph->num_vertices + total_edges * 16;
}

void freeAdjacencyList(AdjacencyList* graph) {
  for (int i = 0; i < graph->num_vertices; i++) {
    Node* current = graph->adjacency_list[i];
    while (current != NULL) {
      Node* temp = current;
      current = current->next;
      free(temp);
    }
  }
  free(graph->adjacency_list);
  free(graph);
}

typedef struct {
  int u;
  int v;
} Edge;

Edge* generateRandomGraph(int num_vertices, int num_edges, int* actual_edges) {
  Edge* edges = (Edge*)malloc(num_edges * sizeof(Edge));
  *actual_edges = 0;
  int max_possible_edges = num_vertices * (num_vertices - 1) / 2;
  int target_edges = (num_edges > max_possible_edges) ? max_possible_edges : num_edges;
  while (*actual_edges < target_edges) {
    int u = rand() % num_vertices;
    int v = rand() % num_vertices;
    if (u != v) {
      int found = 0;
      for (int i = 0; i < *actual_edges; i++) {
        if ((edges[i].u == u && edges[i].v == v) || 
          (edges[i].u == v && edges[i].v == u)) {
          found = 1;
          break;
        }
      }
      if (!found) {
        edges[*actual_edges].u = u;
        edges[*actual_edges].v = v;
        (*actual_edges)++;
      }
    }
  }
  return edges;
}

typedef struct {
  long long memory;
  long long insert_comparisons;
  long long is_connected_comparisons;
  long long adjacent_comparisons;
} PerformanceResult;

PerformanceResult measurePerformanceMatrix(AdjacencyMatrix* graph, Edge* edges, int num_edges) {
  PerformanceResult result = {0, 0, 0, 0};
  long long total_insert = 0;
  for (int i = 0; i < num_edges; i++) {
    addEdgeMatrix(graph, edges[i].u, edges[i].v);
    total_insert += graph->comparisons;
  }
  result.memory = getMemoryUsageMatrix(graph);
  result.insert_comparisons = total_insert;
  long long total_connected = 0;
  int num_tests = (graph->num_vertices < 10) ? graph->num_vertices : 10;
  for (int i = 0; i < num_tests; i++) {
    int u = rand() % graph->num_vertices;
    int v = rand() % graph->num_vertices;
    isConnectedMatrix(graph, u, v);
    total_connected += graph->comparisons;
  }
  result.is_connected_comparisons = total_connected / num_tests;
  long long total_adjacent = 0;
  for (int i = 0; i < num_tests; i++) {
    int u = rand() % graph->num_vertices;
    int count;
    int* adjacent = getAdjacentVerticesMatrix(graph, u, &count);
    total_adjacent += graph->comparisons;
    free(adjacent);
  }
  result.adjacent_comparisons = total_adjacent / num_tests;
  return result;
}

PerformanceResult measurePerformanceList(AdjacencyList* graph, Edge* edges, int num_edges) {
  PerformanceResult result = {0, 0, 0, 0};
  long long total_insert = 0;
  for (int i = 0; i < num_edges; i++) {
        addEdgeList(graph, edges[i].u, edges[i].v);
        total_insert += graph->comparisons;
  }
  result.memory = getMemoryUsageList(graph);
  result.insert_comparisons = total_insert;
  long long total_connected = 0;
  int num_tests = (graph->num_vertices < 10) ? graph->num_vertices : 10;
  for (int i = 0; i < num_tests; i++) {
    int u = rand() % graph->num_vertices;
    int v = rand() % graph->num_vertices;
    isConnectedList(graph, u, v);
    total_connected += graph->comparisons;
  }
  result.is_connected_comparisons = total_connected / num_tests;
  long long total_adjacent = 0;
  for (int i = 0; i < num_tests; i++) {
    int u = rand() % graph->num_vertices;
    int count;
    int* adjacent = getAdjacentVerticesList(graph, u, &count);
    total_adjacent += graph->comparisons;
    free(adjacent);
  }
  result.adjacent_comparisons = total_adjacent / num_tests;
  return result;
}

int main() {
  srand((unsigned)time(NULL));
  int num_vertices = 100;
  int sparse_edges = 100;
  int dense_edges = 4000;

  int actual_sparse, actual_dense;
  Edge* sparse_edges_list = generateRandomGraph(num_vertices, sparse_edges, &actual_sparse);
  Edge* dense_edges_list = generateRandomGraph(num_vertices, dense_edges, &actual_dense);

  AdjacencyMatrix* sparse_matrix = createAdjacencyMatrix(num_vertices);
  PerformanceResult result1 = measurePerformanceMatrix(sparse_matrix, sparse_edges_list, actual_sparse);

  AdjacencyList* sparse_list = createAdjacencyList(num_vertices);
  PerformanceResult result2 = measurePerformanceList(sparse_list, sparse_edges_list, actual_sparse);

  AdjacencyMatrix* dense_matrix = createAdjacencyMatrix(num_vertices);
  PerformanceResult result3 = measurePerformanceMatrix(dense_matrix, dense_edges_list, actual_dense);

  AdjacencyList* dense_list = createAdjacencyList(num_vertices);
  PerformanceResult result4 = measurePerformanceList(dense_list, dense_edges_list, actual_dense);

  printf("케이스 1: 희소그래프 - 인접행렬\n");
  printf("메모리: %lld Bytes\n", result1.memory);
  printf("간선 삽입/삭제 비교: %lld번\n", result1.insert_comparisons);
  printf("두 정점의 연결 확인 비교: %lld번\n", result1.is_connected_comparisons);
  printf("한 노드의 인접 노드 출력 비교: %lld번\n\n", result1.adjacent_comparisons);

  printf("케이스 2: 희소그래프 - 인접리스트\n");
  printf("메모리: %lld Bytes\n", result2.memory);
  printf("간선 삽입/삭제 비교: %lld번\n", result2.insert_comparisons);
  printf("두 정점의 연결 확인 비교: %lld번\n", result2.is_connected_comparisons);
  printf("한 노드의 인접 노드 출력 비교: %lld번\n\n", result2.adjacent_comparisons);

  printf("케이스 3: 밀집그래프 - 인접행렬\n");
  printf("메모리: %lld Bytes\n", result3.memory);
  printf("간선 삽입/삭제 비교: %lld번\n", result3.insert_comparisons);
  printf("두 정점의 연결 확인 비교: %lld번\n", result3.is_connected_comparisons);
  printf("한 노드의 인접 노드 출력 비교: %lld번\n\n", result3.adjacent_comparisons);

  printf("케이스 4: 밀집그래프 - 인접리스트\n");
  printf("메모리: %lld Bytes\n", result4.memory);
  printf("간선 삽입/삭제 비교: %lld번\n", result4.insert_comparisons);
  printf("두 정점의 연결 확인 비교: %lld번\n", result4.is_connected_comparisons);
  printf("한 노드의 인접 노드 출력 비교: %lld번\n\n", result4.adjacent_comparisons);

  freeAdjacencyMatrix(sparse_matrix);
  freeAdjacencyList(sparse_list);
  freeAdjacencyMatrix(dense_matrix);
  freeAdjacencyList(dense_list);
  free(sparse_edges_list);
  free(dense_edges_list);
  return 0;
}