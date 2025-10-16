#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
  int data;
  struct Node* left;
  struct Node* right;
} Node;

typedef struct AVLNode {
  int data;
  struct AVLNode* left;
  struct AVLNode* right;
  int height;
} AVLNode;

Node* createNode(int value) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (!newNode) {
    printf("메모리 할당 실패\n");
    exit(1);
  }
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

AVLNode* createAVLNode(int value) {
  AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
  if (!newNode) {
    printf("AVL 메모리 할당 실패\n");
    exit(1);
  }
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->height = 0;
  return newNode;
}

int treeInsert(Node** root, int value) {
  if (*root == NULL) {
    *root = createNode(value);
    return 1;
  }
  if (value == (*root)->data)
    return 0;
  else if (value < (*root)->data)
    return treeInsert(&((*root)->left), value);
  else
    return treeInsert(&((*root)->right), value);
}

int getHeight(AVLNode* node) {
  return (node == NULL) ? -1 : node->height;
}

int getBalance(AVLNode* node) {
  if (node == NULL) return 0;
  return getHeight(node->left) - getHeight(node->right);
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

AVLNode* rotateRight(AVLNode* y) {
  AVLNode* x = y->left;
  AVLNode* T2 = x->right;
  x->right = y;
  y->left = T2;
  y->height = 1 + max(getHeight(y->left), getHeight(y->right));
  x->height = 1 + max(getHeight(x->left), getHeight(x->right));
  return x;
}

AVLNode* rotateLeft(AVLNode* x) {
  AVLNode* y = x->right;
  AVLNode* T2 = y->left;
  y->left = x;
  x->right = T2;
  x->height = 1 + max(getHeight(x->left), getHeight(x->right));
  y->height = 1 + max(getHeight(y->left), getHeight(y->right));
  return y;
}

AVLNode* avlInsert(AVLNode* node, int value) {
  if (node == NULL) return createAVLNode(value);
  if (value < node->data)
      node->left = avlInsert(node->left, value);
  else
      node->right = avlInsert(node->right, value);
  node->height = 1 + max(getHeight(node->left), getHeight(node->right));
  int balance = getBalance(node);
  if (balance > 1 && value < node->left->data)
      return rotateRight(node);
  if (balance < -1 && value > node->right->data)
      return rotateLeft(node);
  if (balance > 1 && value > node->left->data) {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
  }
  if (balance < -1 && value < node->right->data) {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
  }
  return node;
}

int arrSearch(int arr[], int size, int target) {
  int searchCount = 0;
  for (int i = 0; i < size; i++) {
    searchCount++;
    if (arr[i] == target)
      return searchCount;
  }
  return searchCount;
}

int treeSearch(Node* root, int target) {
  int searchCount = 0;
  Node* current = root;
  while (current != NULL) {
    searchCount++;
    if (target == current->data)
      return searchCount;
    else if (target < current->data)
       current = current->left;
    else
      current = current->right;
  }
  return searchCount;
}

int avlSearch(AVLNode* root, int target) {
  int searchCount = 0;
  AVLNode* current = root;
  while (current != NULL) {
    searchCount++;
    if (target == current->data)
      return searchCount;
    else if (target < current->data)
      current = current->left;
    else
      current = current->right;
  }
  return searchCount;
}

void freeTree(Node* root) {
  if (root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

void freeAVLTree(AVLNode* root) {
  if (root == NULL) return;
  freeAVLTree(root->left);
  freeAVLTree(root->right);
  free(root);
}

void generateData1(int arr[]) {
  int used[10001] = {0};
  int count = 0;
  while (count < 1000) {
    int val = rand() % 10001;
    if (!used[val]) {
      arr[count++] = val;
      used[val] = 1;
    }
  }
}

void generateData2(int arr[]) {
  for (int i = 0; i < 1000; i++)
    arr[i] = i;
}

void generateData3(int arr[]) {
  for (int i = 0; i < 1000; i++)
    arr[i] = 999 - i;
}

void generateData4(int arr[]) {
  for (int i = 0; i < 1000; i++)
    arr[i] = i * (i % 2 + 2);
}

void runExperiment(int data[], const char* label) {
  Node* root = NULL;
  AVLNode* avlRoot = NULL;
  for (int i = 0; i < 1000; i++) {
    treeInsert(&root, data[i]);
    avlRoot = avlInsert(avlRoot, data[i]);
  }
  float arrResult = 0, treeResult = 0, avlResult = 0;
  for (int i = 0; i < 1000; i++) {
    int target = rand() % 10001;
    arrResult += arrSearch(data, 1000, target);
    treeResult += treeSearch(root, target);
    avlResult += avlSearch(avlRoot, target);
  }
  printf("%s 탐색 결과\n", label);
  printf("Array: 평균 %.2f회 탐색\n", arrResult / 1000);
  printf("BST: 평균 %.2f회 탐색\n", treeResult / 1000);
  printf("AVL: 평균 %.2f회 탐색\n\n", avlResult / 1000);

  freeTree(root);
  freeAVLTree(avlRoot);
}

int main() {
  srand((unsigned int)time(NULL));
  int dataArray[1000];
  generateData1(dataArray);
  runExperiment(dataArray, "데이터 (1)");
  generateData2(dataArray);
  runExperiment(dataArray, "데이터 (2)");
  generateData3(dataArray);
  runExperiment(dataArray, "데이터 (3)");
  generateData4(dataArray);
  runExperiment(dataArray, "데이터 (4)");
  return 0;
}
