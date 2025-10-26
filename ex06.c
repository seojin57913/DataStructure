#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VERTICES 100
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

typedef struct Node {
    int dest;
    struct Node* next;
} Node;

typedef struct GraphList {
    Node* head[VERTICES];
} GraphList;

void generate_random_edges(int edges[][2], int num_edges);
void test_adj_matrix(int num_edges, int edges[][2], const char* graph_type);
void test_adj_list(int num_edges, int edges[][2], const char* graph_type);
Node* create_node(int dest);
void add_edge_list(GraphList* graph, int src, int dest);


int main() {
    srand((unsigned int)time(NULL));
    int sparse_edges[SPARSE_EDGES][2];
    generate_random_edges(sparse_edges, SPARSE_EDGES);
    int dense_edges[DENSE_EDGES][2];
    generate_random_edges(dense_edges, DENSE_EDGES);
    test_adj_matrix(SPARSE_EDGES, sparse_edges, "희소 그래프");
    test_adj_list(SPARSE_EDGES, sparse_edges, "희소 그래프");
    test_adj_matrix(DENSE_EDGES, dense_edges, "밀집 그래프");
    test_adj_list(DENSE_EDGES, dense_edges, "밀집 그래프");
    return 0;
}

void generate_random_edges(int edges[][2], int num_edges) {
    char is_connected[VERTICES][VERTICES] = {0};
    int count = 0;
    while (count < num_edges) {
        int u = rand() % VERTICES;
        int v = rand() % VERTICES;
        if (u != v && !is_connected[u][v]) {
            edges[count][0] = u;
            edges[count][1] = v;
            is_connected[u][v] = 1;
            is_connected[v][u] = 1;
            count++;
        }
    }
}

void test_adj_matrix(int num_edges, int edges[][2], const char* graph_type) {
    int(*adj_matrix)[VERTICES] = calloc(VERTICES, sizeof(int[VERTICES]));
    if (!adj_matrix) {
        printf("메모리 할당 실패\n");
        return;
    }
    for (int i = 0; i < num_edges; ++i) {
        adj_matrix[edges[i][0]][edges[i][1]] = 1;
        adj_matrix[edges[i][1]][edges[i][0]] = 1; 
    }
    long memory_usage = (long)VERTICES * VERTICES * sizeof(int);
    int comparisons_insert_delete = 0;
    int u_test = rand() % VERTICES;
    int v_test = rand() % VERTICES;
    if (adj_matrix[u_test][v_test] == 0)
        comparisons_insert_delete++; 
    else 
        comparisons_insert_delete++;
    comparisons_insert_delete++;
    int comparisons_check_edge = 0;
    u_test = edges[0][0];
    v_test = edges[0][1];
    if (adj_matrix[u_test][v_test] == 1) 
        comparisons_check_edge++;
    int comparisons_print_neighbors = 0;
    int node_to_check = rand() % VERTICES;
    for (int i = 0; i < VERTICES; ++i) {
        comparisons_print_neighbors++; 
    }
    printf("케이스: %s - 인접 행렬 \n", graph_type);
    printf("메모리: %ld Bytes\n", memory_usage);
    printf("간선 삽입/삭제 비교: %d번\n", comparisons_insert_delete);
    printf("두 정점의 연결 확인 비교: %d번\n", comparisons_check_edge);
    printf("한 노드의 인접 노드 출력 비교: %d번\n\n", comparisons_print_neighbors);
    free(adj_matrix);
}

void test_adj_list(int num_edges, int edges[][2], const char* graph_type) {
    GraphList* graph = (GraphList*)malloc(sizeof(GraphList));
    for (int i = 0; i < VERTICES; i++) 
        graph->head[i] = NULL;
    for (int i = 0; i < num_edges; i++) {
        add_edge_list(graph, edges[i][0], edges[i][1]);
        add_edge_list(graph, edges[i][1], edges[i][0]);
    }
    long memory_usage = sizeof(Node*) * VERTICES; 
    memory_usage += (long)num_edges * 2 * sizeof(Node); 
    int comparisons_insert_delete = 0;
    int u_test = edges[0][0]; 
    int v_test = edges[0][1];
    Node* current = graph->head[u_test];
    while (current != NULL) {
        comparisons_insert_delete++;
        if (current->dest == v_test)
            break;
        current = current->next;
    }
    current = graph->head[v_test];
    while (current != NULL) {
        comparisons_insert_delete++;
        if (current->dest == u_test) 
            break;
        current = current->next;
    }
    int comparisons_check_edge = 0;
    current = graph->head[u_test];
    while (current != NULL) {
        comparisons_check_edge++;
        if (current->dest == v_test) 
            break;
        current = current->next;
    }
    int comparisons_print_neighbors = 0;
    int node_to_check = rand() % VERTICES;
    current = graph->head[node_to_check];
    while (current != NULL) {
        comparisons_print_neighbors++; 
        current = current->next;
    }
    printf("케이스: %s - 인접 리스트\n", graph_type);
    printf("메모리: %ld Bytes\n", memory_usage);
    printf("간선 삽입/삭제 비교: %d번\n", comparisons_insert_delete); 
    printf("두 정점의 연결 확인 비교: %d번\n", comparisons_check_edge); 
    printf("한 노드의 인접 노드 출력 비교: %d번\n\n", comparisons_print_neighbors); 
    for (int i = 0; i < VERTICES; i++) {
        Node* current = graph->head[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

Node* create_node(int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

void add_edge_list(GraphList* graph, int src, int dest) {
    Node* newNode = create_node(dest);
    newNode->next = graph->head[src];
    graph->head[src] = newNode;
}