#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

long long cmp_count = 0;

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패");
        return NULL;
    }
    char line[MAX_LINE_LEN];
    int capacity = 10, count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            arr = realloc(arr, sizeof(Student) * capacity);
        }
        Student s;
        char* token = strtok(line, ",");
        s.id = atoi(token);
        token = strtok(NULL, ",");
        strcpy(s.name, token);
        token = strtok(NULL, ",");
        s.gender = token[0];
        token = strtok(NULL, ",");
        s.korean = atoi(token);
        token = strtok(NULL, ",");
        s.english = atoi(token);
        token = strtok(NULL, ",");
        s.math = atoi(token);
        arr[count++] = s;
    }
    fclose(fp);
    arr = realloc(arr, sizeof(Student) * count);
    *out_count = count;
    return arr;
}

void shuffle(Student* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Student tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int linear_search(Student* arr, int n, int target_id) {
    for (int i = 0; i < n; i++) {
        cmp_count++;
        if (arr[i].id == target_id)
            return i;
    }
    return -1;
}

int binary_search(Student* arr, int n, int target_id) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        cmp_count++;
        if (arr[mid].id == target_id) return mid;
        else if (arr[mid].id < target_id) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

typedef struct AVL {
    Student data;
    int height;
    struct AVL* left;
    struct AVL* right;
} AVL;

int height(AVL* n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return a > b ? a : b;
}

AVL* newNode(Student s) {
    AVL* n = malloc(sizeof(AVL));
    n->data = s;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

AVL* rightRotate(AVL* y) {
    AVL* x = y->left;
    y->left = x->right;
    x->right = y;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVL* leftRotate(AVL* x) {
    AVL* y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVL* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVL* avl_insert(AVL* node, Student key) {
    if (!node)
        return newNode(key);
    cmp_count++;
    if (key.id < node->data.id)
        node->left = avl_insert(node->left, key);
    else if (key.id > node->data.id)
        node->right = avl_insert(node->right, key);
    else
        return node;
    node->height = max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);
    if (balance > 1 && key.id < node->left->data.id)
        return rightRotate(node);
    if (balance < -1 && key.id > node->right->data.id)
        return leftRotate(node);
    if (balance > 1 && key.id > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key.id < node->right->data.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVL* minValueNode(AVL* node) {
    while (node->left)
        node = node->left;
    return node;
}

AVL* avl_delete(AVL* root, int id) {
    if (!root)
        return root;
    cmp_count++;
    if (id < root->data.id)
        root->left = avl_delete(root->left, id);
    else if (id > root->data.id)
        root->right = avl_delete(root->right, id);
    else {
        if (!root->left || !root->right) {
            AVL* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        else {
            AVL* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = avl_delete(root->right, temp->data.id);
        }
    }
    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

int avl_search(AVL* root, int id) {
    while (root) {
        cmp_count++;
        if (id == root->data.id)
            return 1;
        else if (id < root->data.id) root = root->left;
        else root = root->right;
    }
    return 0;
}

void unsorted_insert(Student* arr, int* n, Student s) {
    arr[*n] = s;
    (*n)++;
}

void unsorted_delete(Student* arr, int* n, int id) {
    int idx = linear_search(arr, *n, id);
    if (idx != -1) {
        arr[idx] = arr[*n - 1];
        (*n)--;
    }
}

void sorted_insert(Student* arr, int* n, Student s) {
    int l = 0, r = *n;
    while (l < r) {
        int mid = (l + r) / 2;
        cmp_count++;
        if (arr[mid].id < s.id) l = mid + 1;
        else r = mid;
    }
    for (int i = *n; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = s;
    (*n)++;
}

void sorted_delete(Student* arr, int* n, int id) {
    int idx = binary_search(arr, *n, id);
    if (idx != -1) {
        for (int i = idx; i < *n - 1; i++)
            arr[i] = arr[i + 1];
        (*n)--;
    }
}

int main() {
    srand(time(NULL));
    int original_n;
    Student* data = load_students("dataset_id_ascending.csv", &original_n);
    int n_unsorted = original_n;
    int n_sorted   = original_n;
    int n_avl      = original_n;
    Student* unsorted = malloc(sizeof(Student) * (original_n + 10));
    memcpy(unsorted, data, sizeof(Student) * original_n);
    shuffle(unsorted, n_unsorted);
    Student* sorted = malloc(sizeof(Student) * (original_n + 10));
    memcpy(sorted, data, sizeof(Student) * original_n);
    AVL* root = NULL;
    cmp_count = 0;
    for (int i = 0; i < original_n; i++)
        root = avl_insert(root, data[i]);
    int target_id = data[original_n / 2].id;
    cmp_count = 0;
    linear_search(unsorted, n_unsorted, target_id);
    printf("[비정렬 배열] 검색 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    unsorted_delete(unsorted, &n_unsorted, target_id);
    printf("[비정렬 배열] 삭제 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    unsorted_insert(unsorted, &n_unsorted, data[original_n / 2]);
    printf("[비정렬 배열] 삽입 비교 횟수: %lld\n\n", cmp_count);
    cmp_count = 0;
    binary_search(sorted, n_sorted, target_id);
    printf("[정렬 배열] 검색 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    sorted_delete(sorted, &n_sorted, target_id);
    printf("[정렬 배열] 삭제 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    sorted_insert(sorted, &n_sorted, data[original_n / 2]);
    printf("[정렬 배열] 삽입 비교 횟수: %lld\n\n", cmp_count);
    cmp_count = 0;
    avl_search(root, target_id);
    printf("[AVL 트리] 검색 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    root = avl_delete(root, target_id);
    printf("[AVL 트리] 삭제 비교 횟수: %lld\n", cmp_count);
    cmp_count = 0;
    root = avl_insert(root, data[original_n / 2]);
    printf("[AVL 트리] 삽입 비교 횟수: %lld\n", cmp_count);
    free(data);
    free(unsorted);
    free(sorted);
    return 0;
}

